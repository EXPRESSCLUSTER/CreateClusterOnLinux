#ifndef _CONF_H_
#define _CONF_H_

enum _func_retval__
{
	CONF_ERR_SUCCESS,
	CONF_ERR_FILE,
	CONF_ERR_COM,
	CONF_ERR_PARAM,
	CONF_ERR_NOTEXIST,
	CONF_ERR_OTHER
};

#define CONF_PATH_LEN		1024

/**
 * prototype
 */
 //clpconfin.c
int init(char *encoding);
int set_value(xmlDocPtr doc, char *path, char *curr);
int del_value(xmlDocPtr doc, char *path);
int rpl_attr(xmlDocPtr doc, char *path, char *attr, char *newname);
int rpl_value(xmlDocPtr doc, char *path, char *value);
xmlNodePtr find_value_node(xmlDocPtr doc, xmlNodePtr node, char *path, char *curr);
xmlNodePtr find_node(xmlDocPtr doc, xmlNodePtr node, char *path, char *curr);
xmlNodePtr find_child_node(xmlDocPtr doc, xmlNodePtr node, char *element, char *attribute);
xmlNodePtr make_child_node(xmlNodePtr node, char *node_name, char *attr_var);

//clpcfset.c
/* add */
int add_cls(char *clsname, char *charset, char *encode, char *os);
int add_srv(char *srvname, char *priority);
int add_hba(char *srvname, char *id, char *port, char *device, char *instance);
int add_dev_lan(char *srvname, char *id, char *ipaddr);
int add_dev_mdc(char *srvname, char *id, char *info);
int add_dev_diskhb(char *srvname, char *id, char *info);
int add_dev_witnesshb(char *srvname, char *id, char *info, char *extend);
int add_dev_bmchb(char *srvname, char *id, char *info);
int add_dev_disknp(char *srvname, char *id, char *guid, char *letter);
int add_dev_pingnp(char *srvname, char *id, char *grpid);
int add_dev_httpnp(char *srvname, char *id, char *info);
int add_hb_lankhb(char *id, char *priority);
int add_hb_lanhb(char *id, char *priority);
int add_hb_bmchb(char *id, char *priority);
int add_hb_diskhb(char *id, char *priority);
int add_hb_witnesshb(char *id, char *priority, char *host);
int add_np_pingnp(char *id, char *priority, char *grpid, char *listid, char *ip); 
int add_np_disknp(char *id, char *priority);
int add_np_httpnp(char *id, char *priority, char *host);
int add_grp(char *grptype, char *grpname); 
int add_grp_param(char *grptype, char *grpname, char *tag, char *param); 
int add_rsc(char *grpname, char *rsctype, char *rscname); 
int add_rsc_dep(char *rsctype, char *rscname, char *depend); 
int add_rsc_param(char *rsctype, char *rscname, char *tag, char *param); 
int add_mon(char *montype, char *monname);
int add_mon_param(char *montype, char *monname, char *tag, char *param); 
int add_cls_param(char *tag, char *param);
int add_cls_webmgr(char *tag, char *param);
int add_obj_num(char *objnum);
/* delete */
int del_srv(char *srvname);
int del_grp(char *grpname);
/* replace */
int rpl_srv(char *curname, char *newname);
int rpl_dev_lan(char *srvname, char *id, char *info);
int rpl_dev_mdc(char *srvname, char *id, char *info);
int rpl_dev_diskhb(char *srvname, char *id, char *info);
int rpl_rsc_param(char *rsctype, char *rscname, char *tag, char *param); 
/* count */
int cal_obj_num(void);
int count_hb(void);
int count_khb(void);
int count_server(void);
int count_group(void);
int count_heartbeat(void);
int count_networkpartition(void);
int count_resource(void);
int count_monitor(void);

int add_rsc_guid(char *rsctype, char *rscname, char *srvname, char *tag, char *guid);
#endif
