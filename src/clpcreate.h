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
xmlNodePtr find_value_node(xmlDocPtr doc, xmlNodePtr node, char *path, char *curr);
xmlNodePtr find_child_node(xmlDocPtr doc, xmlNodePtr node, char *element, char *attribute);
xmlNodePtr make_child_node(xmlNodePtr node, char *node_name, char *attr_var);

//clpcreate.c
int add_cls(char *clsname, char *lang, char *os);
int add_srv(char *srvname, char *priority);
int add_hba(char *srvname, char *id, char *tag, char *param);
int add_ip(char *srvname, char *id, char *ipaddr);
int add_khb(char *id, char *priority);
int add_hb(char *id, char *priority);
int add_bmchb(char *id, char *primority);
int add_diskhb(char *id, char *priority);
int add_diskhbsrv(char *srvname, char *id, char *info);
int add_grp(char *grptype, char *grpname); 
int add_grp_param(char *grptype, char *grpname, char *tag, char *param); 
int add_rsc(char *grpname, char *rsctype, char *rscname); 
int add_rsc_dep(char *rsctype, char *rscname, char *depend); 
int add_rsc_param(char *rsctype, char *rscname, char *tag, char *param); 
int add_rsc_guid(char *rsctype, char *rscname, char *srvname, char *tag, char *guid);
int add_mon(char *montype, char *monname);
int add_mon_param(char *montype, char *monname, char *tag, char *param);
int add_pingnp(char *priority, char *id, char *grpid, char *listid, char *ip); 
int add_disknp(char *npname, char *priority, char *id); 
int add_pingnpsrv(char *srvname, char *id, char *grpid); 
int add_disknpsrv(char *srvname, char *id, char *guid, char *letter);
int add_cls_param(char *tag, char *param);
int add_obj_num(char *objnum);
int cal_obj_num(void);
int rpl_obj_num(char *objnum);
int count_hb(void);
int count_khb(void);
int count_server(void);
int count_group(void);
int count_device(void);
int count_resource(void);
int count_monitor(void);
#endif