/*********************************************************************************************************
* Software License Agreement (BSD License)                                                               *
* Author: Alexandre Westfahl <awestfahl@freediameter.net>						 *
*													 *
* Copyright (c) 2010, Alexandre Westfahl, Teraoka Laboratory (Keio University), and the WIDE Project. 	 *		
*													 *
* All rights reserved.											 *
* 													 *
* Redistribution and use of this software in source and binary forms, with or without modification, are  *
* permitted provided that the following conditions are met:						 *
* 													 *
* * Redistributions of source code must retain the above 						 *
*   copyright notice, this list of conditions and the 							 *
*   following disclaimer.										 *
*    													 *
* * Redistributions in binary form must reproduce the above 						 *
*   copyright notice, this list of conditions and the 							 *
*   following disclaimer in the documentation and/or other						 *
*   materials provided with the distribution.								 *
* 													 *
* * Neither the name of the Teraoka Laboratory nor the 							 *
*   names of its contributors may be used to endorse or 						 *
*   promote products derived from this software without 						 *
*   specific prior written permission of Teraoka Laboratory 						 *
*   													 *
* 													 *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED *
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A *
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 	 *
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 	 *
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR *
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF   *
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.								 *
*********************************************************************************************************/


#include"test_sip.h"



struct disp_hdl * test_sip_MAA_hdl=NULL;
struct disp_hdl * test_sip_LIA_hdl=NULL;
struct disp_hdl * test_sip_UAA_hdl=NULL;
struct disp_hdl * test_sip_SAA_hdl=NULL;
struct disp_hdl * test_sip_PPR_hdl=NULL;
struct disp_hdl * test_sip_RTR_hdl=NULL;


struct disp_hdl * test_sip_default_hdl=NULL;
struct session_handler * ts_sess_hdl;

//configuration stucture
struct ts_conf * ts_conf=NULL;
static struct ts_conf app_sip_conf;

//dictionary of SIP
struct test_sip_dict sip_dict;

int test_sip_default_cb( struct msg ** msg, struct avp * avp, struct session * sess, enum disp_action * act)
{
	TRACE_ENTRY("%p %p %p %p", msg, avp, sess, act);
	
	return 0;
}

username = "awestfahl"
password = "test"
sip_aor = "sip:awestfahl@tera.ics.keio.ac.jp"

#Destination information
destination_realm = "freediameter.net"
destination_sip = "sip:awestfahl@freediameter.net"

void dump_config()
{
	TRACE_DEBUG(FULL,"***Configuration of TEST Diameter-SIP extension***");
	TRACE_DEBUG(FULL,"# username: *%s*",ts_conf->username);
	TRACE_DEBUG(FULL,"# password: *%s*",ts_conf->password);
	TRACE_DEBUG(FULL,"# sip_aor: *%s*",ts_conf->sip_aor);
	TRACE_DEBUG(FULL,"# destination_realm: *%s*",ts_conf->destination_realm);
	TRACE_DEBUG(FULL,"# destination_sip: *%s*",ts_conf->destination_sip);
	TRACE_DEBUG(FULL,"***End of TEST Diameter-SIP configuration extension***");
}

static int ts_conf_init(void)
{
	ts_conf=&test_sip_conf;
	//memset(app_sip_conf, 0, sizeof(struct ts_conf));
	

	return 0;
}

/* entry point */
int as_entry(char * conffile)
{
	TRACE_ENTRY("%p", conffile);
	
	struct dict_object * app=NULL;
	struct disp_when data;
	
	/* Initialize configuration */
	CHECK_FCT( ts_conf_init() );
	
	
	//We parse the configuration file
	if (conffile != NULL) {
		CHECK_FCT( ts_conf_handle(conffile) );
	}
	else
	{
		TRACE_DEBUG(INFO, "We need a configuration file for Diameter-SIP extension. See doc/ for an example.");
	}
	
	//We can dump the configuration extracted from app_sip.conf
	dump_config();
	
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_APPLICATION, APPLICATION_BY_NAME, "Diameter Session Initiation Protocol (SIP) Application", &app, ENOENT) );
	CHECK_FCT( fd_disp_app_support ( app, NULL, 1, 0 ) );
	
	
	
	//We set usefull AVPs 
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Auth-Session-State", &sip_dict.Auth_Session_State, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Auth-Application-Id", &sip_dict.Auth_Application_Id, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "SIP-Auth-Data-Item", &sip_dict.SIP_Auth_Data_Item, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "SIP-Authorization", &sip_dict.SIP_Authorization, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "SIP-Authenticate", &sip_dict.SIP_Authenticate, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "SIP-Number-Auth-Items", &sip_dict.SIP_Number_Auth_Items, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "SIP-Authentication-Scheme", &sip_dict.SIP_Authentication_Scheme, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "SIP-Authentication-Info", &sip_dict.SIP_Authentication_Info, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "SIP-Server-URI", &sip_dict.SIP_Server_URI, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "SIP-Method", &sip_dict.SIP_Method, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "SIP-AOR", &sip_dict.SIP_AOR, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-Realm", &sip_dict.Digest_Realm, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-URI", &sip_dict.Digest_URI, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-Nonce", &sip_dict.Digest_Nonce, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-CNonce", &sip_dict.Digest_CNonce, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-Method", &sip_dict.Digest_Method, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-Nonce-Count", &sip_dict.Digest_Nonce_Count, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-Realm", &sip_dict.Digest_Realm, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-Response", &sip_dict.Digest_Response, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-Response-Auth", &sip_dict.Digest_Response_Auth, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-Username", &sip_dict.Digest_Username, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-Algorithm", &sip_dict.Digest_Algorithm, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-QoP", &sip_dict.Digest_QOP, ENOENT) );
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "User-Name", &sip_dict.User_Name, ENOENT) );
	
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_AVP, AVP_BY_NAME, "Digest-HA1", &sip_dict.Digest_HA1, ENOENT) );
	
	
	//Register Application
	memset(&data, 0, sizeof(data));
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_APPLICATION, APPLICATION_BY_NAME, "Diameter Session Initiation Protocol (SIP) Application", &data.app, ENOENT) );
	
	
	//**Command Codes
	//MAR
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, "Multimedia-Auth-Request", &data.command, ENOENT) );
	CHECK_FCT( fd_disp_register( test_sip_MAR_cb, DISP_HOW_CC, &data, &test_sip_MAR_hdl ) );
	//RTR
	CHECK_FCT( fd_dict_search( fd_g_config->cnf_dict, DICT_COMMAND, CMD_BY_NAME, "Registration-Termination-Request", &data.command, ENOENT) );
	CHECK_FCT( fd_disp_register( test_sip_RTR_cb, DISP_HOW_CC, &data, &test_sip_RTA_hdl ) );
	
	
	//Callback for unexpected messages
	CHECK_FCT( fd_disp_register( test_sip_default_cb, DISP_HOW_APPID, &data, &test_sip_default_hdl ) );
	
	
	//We start database connection
	if(start_mysql_connection())
		return 1;
	
	CHECK_FCT(fd_sess_handler_create(&ds_sess_hdl, free));
	
	return 0;
}

//Cleanup callback
void fd_ext_fini(void)
{
	
	if (test_sip_MAR_cb) {
		(void) fd_disp_unregister(&test_sip_MAR_hdl);
		CHECK_FCT_DO( fd_sess_handler_destroy(&ds_sess_hdl),return);
	}
	
	//We close database connection
	close_mysql_connection();
	

	
	TRACE_ENTRY();
	return ;
}

EXTENSION_ENTRY("test_sip", ts_entry);
