/* WARNING: THIS FILE IS AUTOMATICALLY GENERATED FROM A .SM FILE.
 * Changes made here will certainly be overwritten.
 */

/* 
 * (C) 2003 Clemson University and The University of Chicago 
 *
 * See COPYING in top-level directory.
 */

#include <string.h>
#include <assert.h>
#ifndef WIN32
#include <unistd.h>
#endif

#include "client-state-machine.h"
#include "pvfs2-debug.h"
#include "pvfs2-util.h"
#include "job.h"
#include "gossip.h"
#include "str-utils.h"
#include "pint-cached-config.h"
#include "PINT-reqproto-encode.h"

extern job_context_id pint_client_sm_context;

static int del_eattr_comp_fn(
    void *v_p,
    struct PVFS_server_resp *resp_p,
    int i);

static struct PINT_state_s ST_getattr;
static struct PINT_pjmp_tbl_s ST_getattr_pjtbl[];
static struct PINT_tran_tbl_s ST_getattr_trtbl[];

static PINT_sm_action del_eattr_setup_msgpair(
	struct PINT_smcb *smcb, job_status_s *js_p);

static struct PINT_state_s ST_setup_msgpair;
static struct PINT_pjmp_tbl_s ST_setup_msgpair_pjtbl[];
static struct PINT_tran_tbl_s ST_setup_msgpair_trtbl[];
static struct PINT_state_s ST_xfer_msgpair;
static struct PINT_pjmp_tbl_s ST_xfer_msgpair_pjtbl[];
static struct PINT_tran_tbl_s ST_xfer_msgpair_trtbl[];

static PINT_sm_action del_eattr_cleanup(
	struct PINT_smcb *smcb, job_status_s *js_p);

static struct PINT_state_s ST_cleanup;
static struct PINT_pjmp_tbl_s ST_cleanup_pjtbl[];
static struct PINT_tran_tbl_s ST_cleanup_trtbl[];

struct PINT_state_machine_s pvfs2_client_del_eattr_sm = {
	.name = "pvfs2_client_del_eattr_sm",
	.first_state = &ST_getattr
};

static struct PINT_state_s ST_getattr = {
	 .state_name = "getattr" ,
	 .parent_machine = &pvfs2_client_del_eattr_sm ,
	 .flag = SM_JUMP ,
	 .action.nested = &pvfs2_client_getattr_sm ,
	 .pjtbl = NULL ,
	 .trtbl = ST_getattr_trtbl 
};

static struct PINT_tran_tbl_s ST_getattr_trtbl[] = {
	{ .return_value = 0 ,
	 .next_state = &ST_setup_msgpair },
	{ .return_value = -1 ,
	 .next_state = &ST_cleanup }
};

static struct PINT_state_s ST_setup_msgpair = {
	 .state_name = "setup_msgpair" ,
	 .parent_machine = &pvfs2_client_del_eattr_sm ,
	 .flag = SM_RUN ,
	 .action.func = del_eattr_setup_msgpair ,
	 .pjtbl = NULL ,
	 .trtbl = ST_setup_msgpair_trtbl 
};

static struct PINT_tran_tbl_s ST_setup_msgpair_trtbl[] = {
	{ .return_value = 0 ,
	 .next_state = &ST_xfer_msgpair },
	{ .return_value = -1 ,
	 .next_state = &ST_cleanup }
};

static struct PINT_state_s ST_xfer_msgpair = {
	 .state_name = "xfer_msgpair" ,
	 .parent_machine = &pvfs2_client_del_eattr_sm ,
	 .flag = SM_JUMP ,
	 .action.nested = &pvfs2_msgpairarray_sm ,
	 .pjtbl = NULL ,
	 .trtbl = ST_xfer_msgpair_trtbl 
};

static struct PINT_tran_tbl_s ST_xfer_msgpair_trtbl[] = {
	{ .return_value = -1 ,
	 .next_state = &ST_cleanup }
};

static struct PINT_state_s ST_cleanup = {
	 .state_name = "cleanup" ,
	 .parent_machine = &pvfs2_client_del_eattr_sm ,
	 .flag = SM_RUN ,
	 .action.func = del_eattr_cleanup ,
	 .pjtbl = NULL ,
	 .trtbl = ST_cleanup_trtbl 
};

static struct PINT_tran_tbl_s ST_cleanup_trtbl[] = {
	{ .return_value = -1 ,

	 .flag = SM_TERM }
};

#ifndef WIN32
# 59 "src/client/sysint/sys-del-eattr.sm"
#endif


PVFS_error PVFS_isys_deleattr(
        PVFS_object_ref ref,
        const PVFS_credential *credential,
        PVFS_ds_keyval *key_p,
        PVFS_sys_op_id *op_id,
        PVFS_hint hints,
        void *user_ptr)
{
    int ret = -PVFS_EINVAL;
    PINT_smcb *smcb = NULL;
    PINT_client_sm *sm_p = NULL;

    gossip_debug(GOSSIP_CLIENT_DEBUG, "PINT_isys_del_eattr entered\n");

    if ((ref.handle == PVFS_HANDLE_NULL) ||
       (ref.fs_id == PVFS_FS_ID_NULL))
    {
        gossip_err("invalid (NULL) required argument\n");
	return ret;
    }

    PINT_smcb_alloc(&smcb, PVFS_SYS_DELEATTR,
             sizeof(struct PINT_client_sm),
             client_op_state_get_machine,
             client_state_machine_terminate,
             pint_client_sm_context);
    if (smcb == NULL)
    {
        return -PVFS_ENOMEM;
    }
    sm_p = PINT_sm_frame(smcb, PINT_FRAME_CURRENT);

    PINT_init_msgarray_params(sm_p, ref.fs_id);
    PINT_init_sysint_credential(sm_p->cred_p, credential);
    sm_p->u.deleattr.key_p = key_p;
    sm_p->error_code = 0;
    sm_p->object_ref = ref;
    PVFS_hint_copy(hints, &sm_p->hints);
    

    PINT_SM_GETATTR_STATE_FILL(
                               sm_p->getattr,
                               sm_p->object_ref,
                               PVFS_ATTR_CAPABILITY,
                               PVFS_TYPE_NONE,
                               0);

    return PINT_client_state_machine_post(
            smcb,  op_id, user_ptr);
}

PVFS_error PVFS_sys_deleattr(
        PVFS_object_ref ref,
        const PVFS_credential *credential,
        PVFS_ds_keyval *key_p, 
        PVFS_hint hints)
{
    PVFS_error ret = -PVFS_EINVAL, error = 0;
    PVFS_sys_op_id op_id;

    gossip_debug(GOSSIP_CLIENT_DEBUG, "PVFS_sys_deleattr entered\n");

    ret = PVFS_isys_deleattr(ref, credential,
            key_p, &op_id, hints, NULL);

    if (ret)
    {
        PVFS_perror_gossip("PVFS_isys_deleattr call", ret);
        error = ret;
    }
    else if (!ret && op_id != -1)
    {
        ret = PVFS_sys_wait(op_id, "deleattr", &error);
        if (ret)
        {
             PVFS_perror_gossip("PVFS_sys_wait call", ret);
             error = ret;
        }
        PINT_sys_release(op_id);
    }
    return error;
}

static PINT_sm_action del_eattr_setup_msgpair(
        struct PINT_smcb *smcb, job_status_s *js_p)
{
    struct PINT_client_sm *sm_p = PINT_sm_frame(smcb, PINT_FRAME_CURRENT);
    int ret = -PVFS_EINVAL;
    PINT_sm_msgpair_state *msg_p;

    gossip_debug(GOSSIP_CLIENT_DEBUG,
		"del_eattr state: del_eattr_setup_msgpair\n");

    assert(sm_p->getattr.attr.mask & PVFS_ATTR_CAPABILITY);

    PINT_msgpair_init(&sm_p->msgarray_op);
    msg_p = &sm_p->msgarray_op.msgpair;

    PINT_SERVREQ_DELEATTR_FILL(
            msg_p->req,
            sm_p->getattr.attr.capability,
            sm_p->object_ref.fs_id,
            sm_p->object_ref.handle,
            (*sm_p->u.deleattr.key_p),
            sm_p->hints
            );

    msg_p->fs_id = sm_p->object_ref.fs_id;
    msg_p->handle = sm_p->object_ref.handle;
    msg_p->retry_flag = PVFS_MSGPAIR_RETRY;
    msg_p->comp_fn = del_eattr_comp_fn;

    ret = PINT_cached_config_map_to_server(
            &msg_p->svr_addr,
            msg_p->handle,
            msg_p->fs_id);

    if (ret)
    {
        gossip_err("Failed to map meta server address\n");
        js_p->error_code = 0;
    }

    PINT_sm_push_frame(smcb, 0, &sm_p->msgarray_op);
    return SM_ACTION_COMPLETE;
}

static PINT_sm_action del_eattr_cleanup(
        struct PINT_smcb *smcb, job_status_s *js_p)
{
    struct PINT_client_sm *sm_p = PINT_sm_frame(smcb, PINT_FRAME_CURRENT);
    gossip_debug(GOSSIP_CLIENT_DEBUG,
		"del_eattr state: del_eattr_cleanup\n");
    sm_p->error_code  = js_p->error_code;

    PINT_SM_GETATTR_STATE_CLEAR(sm_p->getattr);

    PINT_SET_OP_COMPLETE;
    return SM_ACTION_TERMINATE;
}

static int del_eattr_comp_fn(
    void *v_p,
    struct PVFS_server_resp *resp_p,
    int i)
{
    int j = 0;
    int ret = 0;
    PINT_smcb *smcb = v_p;
    PINT_client_sm *sm_p = PINT_sm_frame(smcb, PINT_MSGPAIR_PARENT_SM);
    PINT_sm_msgpair_state *msg_p;

    gossip_debug(GOSSIP_CLIENT_DEBUG,
		"del_eattr completion fn: del_eattr_comp_fn\n");

    /* only posted one msgpair */
    assert(i==0);

    /* no return value from del eattrib so just check status */

    if (sm_p->msgarray_op.msgarray[i].op_status != 0)
    {
        ret = sm_p->msgarray_op.msgarray[i].op_status;
    }

    /* if this is the last response, check all of the status values
     * and return error code if any requests failed
     */
    if (i == (sm_p->msgarray_op.count -1))
    {
        foreach_msgpair(&sm_p->msgarray_op, msg_p, j)
        {
            if (msg_p->op_status != 0)
            {
                return(msg_p->op_status);
            }
        }
    }
    return ret;
}

/*
 * Local variables:
 *  mode: c
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 *
 * vim: ft=c ts=8 sts=4 sw=4 expandtab
 */
