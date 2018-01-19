/* WARNING: THIS FILE IS AUTOMATICALLY GENERATED FROM A .SM FILE.
 * Changes made here will certainly be overwritten.
 */

/* 
 * (C) 2001 Clemson University and The University of Chicago 
 *
 * See COPYING in top-level directory.
 */

/* pvfs2_list_eattr_sm
 *
 * This state machine handles incoming server listxattr operations.  These
 * are the operations sent by PVFS_sys_listeattr() among others.
 *
 * The pvfs2_prelude_sm is responsible for reading the actual metadata
 * to begin with, because it does this as part of the permission checking
 * process.
 */

#include <string.h>
#include <assert.h>

#include "server-config.h"
#include "pvfs2-server.h"
#include "pvfs2-internal.h"
#include "pvfs2-attr.h"
#include "pvfs2-types.h"
#include "pvfs2-util.h"
#include "pint-util.h"
#include "pint-eattr.h"

static struct PINT_state_s ST_prelude;
static struct PINT_pjmp_tbl_s ST_prelude_pjtbl[];
static struct PINT_tran_tbl_s ST_prelude_trtbl[];

static PINT_sm_action listeattr_setup_resp(
	struct PINT_smcb *smcb, job_status_s *js_p);

static struct PINT_state_s ST_setup_resp;
static struct PINT_pjmp_tbl_s ST_setup_resp_pjtbl[];
static struct PINT_tran_tbl_s ST_setup_resp_trtbl[];

static PINT_sm_action listeattr_list_eattrib(
	struct PINT_smcb *smcb, job_status_s *js_p);

static struct PINT_state_s ST_list_eattrib;
static struct PINT_pjmp_tbl_s ST_list_eattrib_pjtbl[];
static struct PINT_tran_tbl_s ST_list_eattrib_trtbl[];

static PINT_sm_action listeattr_check_resp(
	struct PINT_smcb *smcb, job_status_s *js_p);

static struct PINT_state_s ST_check_resp;
static struct PINT_pjmp_tbl_s ST_check_resp_pjtbl[];
static struct PINT_tran_tbl_s ST_check_resp_trtbl[];
static struct PINT_state_s ST_final_response;
static struct PINT_pjmp_tbl_s ST_final_response_pjtbl[];
static struct PINT_tran_tbl_s ST_final_response_trtbl[];

static PINT_sm_action listeattr_cleanup(
	struct PINT_smcb *smcb, job_status_s *js_p);

static struct PINT_state_s ST_cleanup;
static struct PINT_pjmp_tbl_s ST_cleanup_pjtbl[];
static struct PINT_tran_tbl_s ST_cleanup_trtbl[];

struct PINT_state_machine_s pvfs2_list_eattr_sm = {
	.name = "pvfs2_list_eattr_sm",
	.first_state = &ST_prelude
};

static struct PINT_state_s ST_prelude = {
	 .state_name = "prelude" ,
	 .parent_machine = &pvfs2_list_eattr_sm ,
	 .flag = SM_JUMP ,
	 .action.nested = &pvfs2_prelude_sm ,
	 .pjtbl = NULL ,
	 .trtbl = ST_prelude_trtbl 
};

static struct PINT_tran_tbl_s ST_prelude_trtbl[] = {
	{ .return_value = 0 ,
	 .next_state = &ST_setup_resp },
	{ .return_value = -1 ,
	 .next_state = &ST_final_response }
};

static struct PINT_state_s ST_setup_resp = {
	 .state_name = "setup_resp" ,
	 .parent_machine = &pvfs2_list_eattr_sm ,
	 .flag = SM_RUN ,
	 .action.func = listeattr_setup_resp ,
	 .pjtbl = NULL ,
	 .trtbl = ST_setup_resp_trtbl 
};

static struct PINT_tran_tbl_s ST_setup_resp_trtbl[] = {
	{ .return_value = 0 ,
	 .next_state = &ST_list_eattrib },
	{ .return_value = -1 ,
	 .next_state = &ST_final_response }
};

static struct PINT_state_s ST_list_eattrib = {
	 .state_name = "list_eattrib" ,
	 .parent_machine = &pvfs2_list_eattr_sm ,
	 .flag = SM_RUN ,
	 .action.func = listeattr_list_eattrib ,
	 .pjtbl = NULL ,
	 .trtbl = ST_list_eattrib_trtbl 
};

static struct PINT_tran_tbl_s ST_list_eattrib_trtbl[] = {
	{ .return_value = -1 ,
	 .next_state = &ST_check_resp }
};

static struct PINT_state_s ST_check_resp = {
	 .state_name = "check_resp" ,
	 .parent_machine = &pvfs2_list_eattr_sm ,
	 .flag = SM_RUN ,
	 .action.func = listeattr_check_resp ,
	 .pjtbl = NULL ,
	 .trtbl = ST_check_resp_trtbl 
};

static struct PINT_tran_tbl_s ST_check_resp_trtbl[] = {
	{ .return_value = -1 ,
	 .next_state = &ST_final_response }
};

static struct PINT_state_s ST_final_response = {
	 .state_name = "final_response" ,
	 .parent_machine = &pvfs2_list_eattr_sm ,
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
	 .parent_machine = &pvfs2_list_eattr_sm ,
	 .flag = SM_RUN ,
	 .action.func = listeattr_cleanup ,
	 .pjtbl = NULL ,
	 .trtbl = ST_cleanup_trtbl 
};

static struct PINT_tran_tbl_s ST_cleanup_trtbl[] = {
	{ .return_value = -1 ,

	 .flag = SM_TERM }
};

#ifndef WIN32
# 71 "src/server/list-eattr.sm"
#endif


/*
 * listeattr_setup_resp()
 * Set up the response - allocate needed resources
 */
static PINT_sm_action listeattr_setup_resp(
        struct PINT_smcb *smcb, job_status_s *js_p)
{
    struct PINT_server_op *s_op = PINT_sm_frame(smcb, PINT_FRAME_CURRENT);
    int i, tsz;
    gossip_debug(GOSSIP_LISTEATTR_DEBUG, "listeattr requesting %d keys\n",
            s_op->req->u.listeattr.nkey);

    js_p->error_code = 0;

    /* ensure not too many keys were requested */
    if( s_op->req->u.listeattr.nkey > PVFS_MAX_XATTR_LISTLEN )
    {
        js_p->error_code = -PVFS_EINVAL;
        return SM_ACTION_COMPLETE;
    }

    /* enforce that no key size be larger than PVFS_MAX_XATTR_NAMELEN.
     * Otherwise, when a blind memcpy happens inside dbpf based on the key 
     * size we won't over run our fixed length buffer. fixed buffer size is: 
     * PVFS_NAME_MAX ==  DBPF_MAX_KEY_LENGTH == PVFS_MAX_XATTR_NAMELEN */
    for( i = 0; i < s_op->req->u.listeattr.nkey; i++)
    {
        if( s_op->req->u.listeattr.keysz[i] > PVFS_MAX_XATTR_NAMELEN )
        {
             gossip_debug(GOSSIP_LISTEATTR_DEBUG, "%s: requested key %d "
                          "size of %ld is greater than maximum of %d\n",
                          __func__, i, 
                          ((long int)s_op->req->u.listeattr.keysz[i]),
                          PVFS_MAX_XATTR_NAMELEN );
            js_p->error_code = -PVFS_EINVAL;
            return SM_ACTION_COMPLETE;
        }
    }

    s_op->resp.u.listeattr.key =
        malloc(s_op->req->u.listeattr.nkey * sizeof(PVFS_ds_keyval));
    if (!s_op->resp.u.listeattr.key)
    {
        js_p->error_code = -PVFS_ENOMEM;
        return SM_ACTION_COMPLETE;
    }

    s_op->resp.u.listeattr.nkey = s_op->req->u.listeattr.nkey;

    for (i = 0, tsz = 0; i < s_op->req->u.listeattr.nkey; i++)
    {
        tsz += s_op->req->u.listeattr.keysz[i];
    }
    s_op->u.eattr.buffer = malloc(tsz);
    if (!s_op->u.eattr.buffer)
    {
        s_op->resp.u.listeattr.nkey = 0;
        free (s_op->resp.u.listeattr.key);
        js_p->error_code = -PVFS_ENOMEM;
        return SM_ACTION_COMPLETE;
    }
    gossip_debug(GOSSIP_LISTEATTR_DEBUG,"listeattr buffer size %d bytes\n",
            tsz);
    for (i = 0, tsz = 0; i < s_op->req->u.listeattr.nkey; i++)
    {
        s_op->resp.u.listeattr.key[i].buffer_sz =
            s_op->req->u.listeattr.keysz[i];
        s_op->resp.u.listeattr.key[i].buffer =
            (char *)s_op->u.eattr.buffer + tsz;
        tsz += s_op->req->u.listeattr.keysz[i];
    }
    return SM_ACTION_COMPLETE;
}

/*
 * listeattr_list_eattrib()
 * Here is where the eattrib get listed.
 */
static PINT_sm_action listeattr_list_eattrib(
        struct PINT_smcb *smcb, job_status_s *js_p)
{
    struct PINT_server_op *s_op = PINT_sm_frame(smcb, PINT_FRAME_CURRENT);
    int ret = -PVFS_EINVAL;
    job_id_t i;

    js_p->error_code = 0;

    ret = job_trove_keyval_iterate_keys(
        s_op->req->u.listeattr.fs_id,
        s_op->req->u.listeattr.handle,
        s_op->req->u.listeattr.token,
        s_op->resp.u.listeattr.key,
        s_op->req->u.listeattr.nkey,
        0,
        NULL,
        smcb,
        0,
        js_p,
        &i,
        server_job_context, s_op->req->hints);

    return ret;
}

#if 0
static void print_string(char *buffer, size_t length)
{
    int i;
    if (length == 0 || buffer == NULL)
    {
        return;
    }
    for (i = 0; i < length; i++)
    {
        printf("%c", buffer[i]);
    }
    printf("\n");
    return;
}
#endif

/* listeattr_check_resp()
 *
 * checks what attributes we found, handles errors, filters for appropriate
 * extended attribute name spaces
 *
 * NOTE: all attributes should fall into one of the name spaces defined in 
 * the PINT_eattr_namespaces array.  If we find a key that does not, then we
 * will translate it so that it appears in the "system.pvfs2." name
 * space on the client side.
 */
static PINT_sm_action listeattr_check_resp(
        struct PINT_smcb *smcb, job_status_s *js_p)
{
    struct PINT_server_op *s_op = PINT_sm_frame(smcb, PINT_FRAME_CURRENT);
    int num_found = 0;
    int i = 0;

    /* Nothing was requested? then fill token to hold the max available keys */
    if (s_op->resp.u.listeattr.nkey == 0)
    {
        s_op->resp.u.listeattr.token = js_p->count;
        js_p->error_code = 0;
        return SM_ACTION_COMPLETE;
    }

    /* how many entries did we find that we can process? */
    num_found = js_p->count;
    if(num_found > s_op->resp.u.listeattr.nkey)
    {
        num_found = s_op->resp.u.listeattr.nkey;
    }

    s_op->resp.u.listeattr.token = js_p->position;
    s_op->resp.u.listeattr.nkey = num_found;

    if(num_found == 0)
    {
        /* there aren't any extended attr's; go ahead and return */
        js_p->error_code = 0;
        return SM_ACTION_COMPLETE;
    }

    /* iterate through the keys that we found */
    for(i=0; i<num_found; i++)
    {
        js_p->error_code = PINT_eattr_list_access(
            &s_op->resp.u.listeattr.key[i],
            NULL);
        if(js_p->error_code != 0)
        {
            return SM_ACTION_COMPLETE;
        }

        s_op->resp.u.listeattr.key[i].buffer_sz =
            s_op->resp.u.listeattr.key[i].read_sz;
    }

    js_p->error_code = 0;
    return SM_ACTION_COMPLETE;
}

/* listeattr_cleanup()
 * free resources alloc'd by state machine
 */
static PINT_sm_action listeattr_cleanup(
        struct PINT_smcb *smcb, job_status_s *js_p)
{
    struct PINT_server_op *s_op = PINT_sm_frame(smcb, PINT_FRAME_CURRENT);
    if (s_op->resp.u.listeattr.key)
        free(s_op->resp.u.listeattr.key);
    if (s_op->u.eattr.buffer)
        free(s_op->u.eattr.buffer);
    return(server_state_machine_complete(smcb));
}

PINT_GET_OBJECT_REF_DEFINE(listeattr);

static int perm_listeattr(PINT_server_op *s_op)
{
    int ret;

    ret = 0;

    return ret;
}

struct PINT_server_req_params pvfs2_list_eattr_params =
{
    .string_name = "listeattr",
    .perm = perm_listeattr,
    .sched_policy = PINT_SERVER_REQ_SCHEDULE,
    .get_object_ref = PINT_get_object_ref_listeattr,
    .state_machine = &pvfs2_list_eattr_sm
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

