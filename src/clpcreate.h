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
int add_ip(char *srvname, char *id, char *ipaddr);
int add_hb(char *id, char *priority);
/* 
int add_hba(IN char *srvname, IN char *id, IN char *tag, IN char *param);
int add_diskhb(IN char *id, IN char *priority);
int add_diskhbsrv(IN char *srvname, IN char *id, IN char *info);
int add_grp(IN char *grptype, IN char *grpname);
int add_rsc(IN char *grpname, IN char *rsctype, IN char *rscname);
int add_rsc_dep(IN char *rsctype, IN char *rscname, IN char *depend);
int add_rsc_param(IN char *rsctype, IN char *rscname, IN char *tag, IN char *param);
int add_rsc_guid(IN char *rsctype, IN char *rscname, IN char *srvname, IN char *tag, IN char *guid);
int add_mon(IN char *montype, IN char *monname);
int add_mon_param(IN char *montype, IN char *monname, IN char *tag, IN char *param);
int add_pingnp(IN char *npname, IN char *priority, IN char *id, IN char *grpid, IN char *listid, IN char *ip);
int add_disknp(IN char *npname, IN char *priority, IN char *id);
int add_pingnpsrv(IN char *srvname, IN char *id, IN char *grpid);
int add_disknpsrv(IN char *srvname, IN char *id, IN char *guid, IN char *letter);
*/

#endif