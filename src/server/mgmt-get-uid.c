/* WARNING: THIS FILE IS AUTOMATICALLY GENERATED FROM A .SM FILE.
 * Changes made here will certainly be overwritten.
 */

/* 
 * (C) 2001 Clemson University and The University of Chicago 
 *
 * See COPYING in top-level directory.
 */
#include <stdio.h>

#include "pvfs2-server.h"
#include "pvfs2-internal.h"
#include "pint-uid-mgmt.h"
#include "pint-util.h"

/* static array used to quickly pull uid stats from the server */
static PVFS_uid_info_s *static_array = NULL;

static struct PINT_state_s ST_prelude;
static struct PINT_pjmp_tbl_s ST_prelude_pjtbl[];
static struct PINT_tran_tbl_s ST_prelude_trtbl[];

static PINT_sm_action uid_mgmt_do_work(
	struct PINT_smcb *smcb, job_status_s *js_p);

static struct PINT_state_s ST_do_work;
static struct PINT_pjmp_tbl_s ST_do_work_pjtbl[];
static struct PINT_tran_tbl_s ST_do_work_trtbl[];
static struct PINT_state_s ST_final_response;
static struct PINT_pjmp_tbl_s ST_final_response_pjtbl[];
static struct PINT_tran_tbl_s ST_final_response_trtbl[];

static PINT_sm_action uid_mgmt_cleanup(
	struct PINT_smcb *smcb, job_status_s *js_p);

static struct PINT_state_s ST_cleanup;
static struct PINT_pjmp_tbl_s ST_cleanup_pjtbl[];
static struct PINT_tran_tbl_s ST_cleanup_trtbl[];

struct PINT_state_machine_s pvfs2_uid_mgmt_sm = {
	.name = "pvfs2_uid_mgmt_sm",
	.first_state = &ST_prelude
};

static struct PINT_state_s ST_prelude = {
	 .state_name = "prelude" ,
	 .parent_machine = &pvfs2_uid_mgmt_sm ,
	 .flag = SM_JUMP ,
	 .action.nested = &pvfs2_prelude_sm ,
	 .pjtbl = NULL ,
	 .trtbl = ST_prelude_trtbl 
};

static struct PINT_tran_tbl_s ST_prelude_trtbl[] = {
	{ .return_value = -1 ,
	 .next_state = &ST_do_work }
};

static struct PINT_state_s ST_do_work = {
	 .state_name = "do_work" ,
	 .parent_machine = &pvfs2_uid_mgmt_sm ,
	 .flag = SM_RUN ,
	 .action.func = uid_mgmt_do_work ,
	 .pjtbl = NULL ,
	 .trtbl = ST_do_work_trtbl 
};

static struct PINT_tran_tbl_s ST_do_work_trtbl[] = {
	{ .return_value = -1 ,
	 .next_state = &ST_final_response }
};

static struct PINT_state_s ST_final_response = {
	 .state_name = "final_response" ,
	 .parent_machine = &pvfs2_uid_mgmt_sm ,
	 .flag = SM_JUMP ,
	 .action.nested = &pvfs2_final_response_sm ,
	 .pjtbl = NULL ,
	 .trtbl = ST_final_response_trtbl 
};

static struct PINT_tran_tbl_s ST_final_response_trtbl[] = {
	{ .return_value = -1 ,
	 .next_state = &ST_cleanup }
};

static struct PINT_state_s ST_cleanup = {
	 .state_name = "cleanup" ,
	 .parent_machine = &pvfs2_uid_mgmt_sm ,
	 .flag = SM_RUN ,
	 .action.func = uid_mgmt_cleanup ,
	 .pjtbl = NULL ,
	 .trtbl = ST_cleanup_trtbl 
};

static struct PINT_tran_tbl_s ST_cleanup_trtbl[] = {
	{ .return_value = -1 ,

	 .flag = SM_TERM }
};

#ifndef WIN32
# 44 "src/server/mgmt-get-uid.sm"
#endif


/** uid_mgmt_cleanup()
 *
 * cleans up any resources consumed by this state machine and ends
 * execution of the machine
 */
static PINT_sm_action uid_mgmt_cleanup(
        struct PINT_smcb *smcb, job_status_s *js_p)
{
    struct PINT_server_op *s_op = PINT_sm_frame(smcb, PINT_FRAME_CURRENT);

    if(s_op->resp.u.mgmt_get_uid.uid_info_array)
        free(s_op->resp.u.mgmt_get_uid.uid_info_array);

    return(server_state_machine_complete(smcb));
}

/** uid_mgmt_do_work()
 *
 * gathers uid statistics from server and builds response
 */
static PINT_sm_action uid_mgmt_do_work(
        struct PINT_smcb *smcb, job_status_s *js_p)
{
    struct PINT_server_op *s_op = PINT_sm_frame(smcb, PINT_FRAME_CURRENT);
    int i;
    struct timeval oldest;

    /* allocate memory for a static array, used to quickly pull the uid
     * statistics from the server without blocking access to the uid lists
     */ 
    if (!static_array)
    {
        static_array = (PVFS_uid_info_s *)
                       malloc(UID_MGMT_MAX_HISTORY * sizeof(PVFS_uid_info_s));
        if (!static_array)
        {
            s_op->resp.u.mgmt_get_uid.uid_info_array = NULL;
            js_p->error_code = -PVFS_ENOMEM;
            return SM_ACTION_COMPLETE; 
        }
    }

    /* gather all uid statistics and store them in the static array */
    PINT_dump_all_uid_stats(static_array);

    /* get a timestamp for the max history we want to look through */
    PINT_util_get_current_timeval(&oldest);
    oldest.tv_sec -= s_op->req->u.mgmt_get_uid.history;

    /* scan uid stats to determine how much info we need to send back */
    for (i = 0; i < UID_MGMT_MAX_HISTORY; i++)
    {
        if((static_array[i].count == 0) || 
            !(IN_UID_HISTORY(static_array[i].tv, oldest)))
        {
            break;
        }
    }

    /* allocate memory for and fill in our response back */
    s_op->resp.u.mgmt_get_uid.uid_info_array_count = i;
    s_op->resp.u.mgmt_get_uid.uid_info_array = (PVFS_uid_info_s *)
                 malloc(i * sizeof(PVFS_uid_info_s));
    if (!(s_op->resp.u.mgmt_get_uid.uid_info_array))
    {
        js_p->error_code = -PVFS_ENOMEM;
        return SM_ACTION_COMPLETE; 
    }

    memcpy(s_op->resp.u.mgmt_get_uid.uid_info_array, static_array,
      (s_op->resp.u.mgmt_get_uid.uid_info_array_count * sizeof(PVFS_uid_info_s)));

    js_p->error_code = 0;
    return SM_ACTION_COMPLETE;
}

static int perm_mgmt_get_uid(PINT_server_op *s_op)
{
    return 0;
}

struct PINT_server_req_params pvfs2_uid_mgmt_params =
{
    .string_name = "mgmt_get_uid",
    .perm = perm_mgmt_get_uid,
    .state_machine = &pvfs2_uid_mgmt_sm
};


/*
 * Local variables:
 *  mode: c
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 *
 * vim: ft=c ts=8 sts=4 sw=4 expandtab
 */