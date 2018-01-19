/* WARNING: THIS FILE IS AUTOMATICALLY GENERATED FROM A .SM FILE.
 * Changes made here will certainly be overwritten.
 */

/* 
 * (C) 2001 Clemson University and The University of Chicago 
 *
 * See COPYING in top-level directory.
 */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#include "pvfs2-server.h"
#include "pvfs2-internal.h"
#include "pint-perf-counter.h"
#include "server-config.h"
#include "pint-security.h"


static PINT_sm_action perf_update_do_work(
	struct PINT_smcb *smcb, job_status_s *js_p);

static struct PINT_state_s ST_do_work;
static struct PINT_pjmp_tbl_s ST_do_work_pjtbl[];
static struct PINT_tran_tbl_s ST_do_work_trtbl[];

static PINT_sm_action perf_update_error(
	struct PINT_smcb *smcb, job_status_s *js_p);

static struct PINT_state_s ST_error;
static struct PINT_pjmp_tbl_s ST_error_pjtbl[];
static struct PINT_tran_tbl_s ST_error_trtbl[];

struct PINT_state_machine_s pvfs2_perf_update_sm = {
	.name = "pvfs2_perf_update_sm",
	.first_state = &ST_do_work
};

static struct PINT_state_s ST_do_work = {
	 .state_name = "do_work" ,
	 .parent_machine = &pvfs2_perf_update_sm ,
	 .flag = SM_RUN ,
	 .action.func = perf_update_do_work ,
	 .pjtbl = NULL ,
	 .trtbl = ST_do_work_trtbl 
};

static struct PINT_tran_tbl_s ST_do_work_trtbl[] = {
	{ .return_value = 0 ,
	 .next_state = &ST_do_work },
	{ .return_value = -1 ,
	 .next_state = &ST_error }
};

static struct PINT_state_s ST_error = {
	 .state_name = "error" ,
	 .parent_machine = &pvfs2_perf_update_sm ,
	 .flag = SM_RUN ,
	 .action.func = perf_update_error ,
	 .pjtbl = NULL ,
	 .trtbl = ST_error_trtbl 
};

static struct PINT_tran_tbl_s ST_error_trtbl[] = {
	{ .return_value = -1 ,

	 .flag = SM_TERM }
};

#ifndef WIN32
# 37 "src/server/perf-update.sm"
#endif


/** perf_update_error()
 *
 * cleans up any resources consumed by this state machine and ends
 * execution of the machine
 */
static PINT_sm_action perf_update_error(
        struct PINT_smcb *smcb, job_status_s *js_p)
{
    gossip_err("Error: stopping server performance monitoring.\n");

    PINT_perf_finalize(PINT_server_pc);

    return(server_state_machine_complete(smcb));
}

/** perf_update_do_work()
 *
 * resets counters, updates metrices, etc- this is intended to be called
 * repeatedly on a regular interval
 */
static PINT_sm_action perf_update_do_work(
        struct PINT_smcb *smcb, job_status_s *js_p)
{
    job_id_t tmp_id;
    uint64_t current_mask = 0;
    int current_debug_on = 0;
    char* tmp_text;
    char* ptr;
    char* token;
    char delim[] = "\n";

#if 0
    PINT_STATE_DEBUG("do_work");
#endif
    
    /* log current statistics if the gossip mask permits */
    gossip_get_debug_mask(&current_debug_on, &current_mask);
    if(current_mask & GOSSIP_PERFCOUNTER_DEBUG)
    {
        tmp_text = PINT_perf_generate_text(PINT_server_pc, 4096);
        if(tmp_text)
        {
            token = strtok_r(tmp_text, delim, &ptr);
            while(token)
            {
                gossip_perf_log("%s\n", token);
                token = strtok_r(NULL, delim, &ptr);
            }
            free(tmp_text);
        }
    }
    
    /* roll over to next set of statistics */
    PINT_perf_rollover(PINT_server_pc);
	
    /* post another timer */
    return(job_req_sched_post_timer(PINT_server_pc->interval,
	        smcb,
	        0,
	        js_p,
	        &tmp_id,
	        server_job_context));
}

static int perm_perf_update(PINT_server_op *s_op)
{
    int ret;

    ret = 0;

    return ret;
}

struct PINT_server_req_params pvfs2_perf_update_params =
{
    .string_name = "perf_update",
    .perm = perm_perf_update,
    .state_machine = &pvfs2_perf_update_sm
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
