#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libxml/tree.h>
#include <libxml/parser.h>

#include "clpcreate.h"

static xmlDocPtr	g_doc = NULL;
static char			*g_encoding[16];
int		g_webmgr = 0;
char	g_ip[1024];
char	g_subnet[1024];

int
main(
	int argc,
	char **argv
)
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr	new_node = NULL;
	int			ret;
	char		obj_num[16];

	if (!strcmp(argv[1], "init"))
	{
		init(argv[2]);
	}
	else if (strcmp(argv[1], "add") & strcmp(argv[1], "rpl") & strcmp(argv[1], "rmv"))
	{
		printf("invalid parameter (argv[1]: %s) \n", argv[1]);
		goto func_exit;
	}

	g_doc = xmlParseFile("clp.conf");
	strcpy(g_encoding, g_doc->encoding);

	if (!strcmp(argv[1], "add"))
	{
		if (!strcmp(argv[2], "cls"))
		{
			add_cls(argv[3], argv[4], argv[5], argv[6]);
		}
		else if (!strcmp(argv[2], "srv"))
		{
			add_srv(argv[3], argv[4]);
		}
		else if (!strcmp(argv[2], "hba"))
		{
			add_hba(argv[3], argv[4], argv[5], argv[6]);
		}
		else if (!strcmp(argv[2], "ip"))
		{
			add_ip(argv[3], argv[4], argv[5]);
		}
		else if (!strcmp(argv[2], "khb"))
		{
			add_khb(argv[3], argv[4]);
		}
		else if (!strcmp(argv[2], "hb"))
		{
			add_hb(argv[3], argv[4]);
		}
		else if (!strcmp(argv[2], "bmchb"))
		{
			add_bmchb(argv[3], argv[4]);
		}
		else if (!strcmp(argv[2], "bmchbsrv"))
		{
			add_bmchbsrv(argv[3], argv[4], argv[5]);
		}
		else if (!strcmp(argv[2], "diskhb"))
		{
			add_diskhb(argv[3], argv[4]);
		}
		else if (!strcmp(argv[2], "diskhbsrv"))
		{
			add_diskhbsrv(argv[3], argv[4], argv[5]);
		}
		else if (!strcmp(argv[2], "mdc"))
		{
			add_mdc(argv[3], argv[4], argv[5]);
		}
		else if (!strcmp(argv[2], "np"))
		{
			if (!strcmp(argv[3], "disk"))
			{
				add_disknp(argv[4], argv[5]);
			}
			else if (!strcmp(argv[3], "ping"))
			{
				add_pingnp(argv[4], argv[5], argv[6], argv[7], argv[8]);
			}
			else
			{
				/* error */
			}
		}
		else if (!strcmp(argv[2], "npsrv"))
		{
			if (!strcmp(argv[3], "disk"))
			{
				add_disknpsrv(argv[4], argv[5], argv[6], argv[7]);
			}
			else if (!strcmp(argv[3], "ping"))
			{
				add_pingnpsrv(argv[4], argv[5], argv[6]);
			}
			else
			{
				/* error */
			}
		}
		else if (!strcmp(argv[2], "grp"))
		{
			add_grp(argv[3], argv[4]);
		}
		else if (!strcmp(argv[2], "grpparam"))
		{
			add_grp_param(argv[3], argv[4], argv[5], argv[6]);
		}
		else if (!strcmp(argv[2], "rsc"))
		{
			add_rsc(argv[3], argv[4], argv[5]);
		}
		else if (!strcmp(argv[2], "rscdep"))
		{
			add_rsc_dep(argv[3], argv[4], argv[5]);
		}
		else if (!strcmp(argv[2], "rscparam"))
		{
			add_rsc_param(argv[3], argv[4], argv[5], argv[6]);
		}
		else if (!strcmp(argv[2], "rscguid"))
		{
			add_rsc_guid(argv[3], argv[4], argv[5], argv[6], argv[7]);
		}
		else if (!strcmp(argv[2], "mon"))
		{
			add_mon(argv[3], argv[4]);
		}
		else if (!strcmp(argv[2], "monparam"))
		{
			add_mon_param(argv[3], argv[4], argv[5], argv[6]);
		}
		else if (!strcmp(argv[2], "clsparam"))
		{
			add_cls_param(argv[3], argv[4]);
		}
		else if (!strcmp(argv[2], "webmgr"))
		{
			g_webmgr = 1;
			add_cls_webmgr(argv[3], argv[4]);
		}
		else
		{
		}
	} else if (!strcmp(argv[1], "rpl")) {

	} else if (!strcmp(argv[1], "rmv")) {

	}

	ret = cal_obj_num();
	sprintf(obj_num, "%d", ret);
	add_obj_num(obj_num);

	xmlSaveFormatFileEnc("clp.conf", g_doc, g_encoding, 1);

	xmlFreeDoc(g_doc);
	xmlCleanupParser();

func_exit:
	return 0;
}


int
add_cls(
	char *clsname,
	char *charset,
	char *encode,
	char *os
)
{
	int		ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	ret = set_value(g_doc, "/root/all/charset", charset);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	ret = set_value(g_doc, "/root/all/serveros", os);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	ret = set_value(g_doc, "/root/all/encode", encode);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	ret = set_value(g_doc, "/root/cluster/name", clsname);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	ret = set_value(g_doc, "/root/cluster/comment", "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_srv(
 	char *srvname,
	char *priority
)
{
	char	path[CONF_PATH_LEN];
	int		ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;
	
	sprintf(path, "/root/server@%s/priority", srvname);
	ret = set_value(g_doc, path, priority);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_hba(
	char *srvname,
	char *id,
	char *tag,
	char *param
)
{
	char	path[CONF_PATH_LEN];
	int		ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/server@%s/hba@%s/%s", srvname, id, tag);
	ret = set_value(g_doc, path, param);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_ip(
	char *srvname,
	char *id,
	char *ipaddr
)
{
	char 		path[CONF_PATH_LEN];
	char		type[16];
	int			i;
	int			ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;
	i = atoi(id);
	if (i < 600) {
		strcpy(type, "lan");
	} else {
		strcpy(type, "bmc");
	}
	
	sprintf(path, "/root/server@%s/device@%s/type", srvname, id);
	ret = set_value(g_doc, path, type);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/info", srvname, id);
	ret = set_value(g_doc, path, ipaddr);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_khb(
	char *id,
	char *priority
)
{
	char 		path[CONF_PATH_LEN];
	char		dev_number[16];
	int			i;
	int			ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	i = count_khb();
	i++;
	sprintf(dev_number, "%d", i);
	
	sprintf(path, "/root/heartbeat/types@lankhb");
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/heartbeat/lankhb@lankhb%s/priority", dev_number);
	ret = set_value(g_doc, path, priority);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/heartbeat/lankhb@lankhb%s/device", dev_number);
	ret = set_value(g_doc, path, id);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_hb(
	char *id,
	char *priority
)
{
	char 		path[CONF_PATH_LEN];
	char		dev_number[16];
	int			i;
	int			ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	i = count_hb();
	i++;
	sprintf(dev_number, "%d", i);
	
	sprintf(path, "/root/heartbeat/types@lanhb");
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/heartbeat/lanhb@lanhb%s/priority", dev_number);
	ret = set_value(g_doc, path, priority);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/heartbeat/lanhb@lanhb%s/device", dev_number);
	ret = set_value(g_doc, path, id);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}

int
add_bmchb(
	char *id,
	char *priority
)
{
	char 		path[CONF_PATH_LEN];
	char		dev_number[16];
	int			i;
	int			ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	i = atoi(id) - 600;
	i++;
	sprintf(dev_number, "%d", i);
	
	sprintf(path, "/root/heartbeat/types@bmchb");
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/heartbeat/bmchb@bmchb%s/priority", dev_number);
	ret = set_value(g_doc, path, priority);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/heartbeat/bmchb@bmchb%s/device", dev_number);
	ret = set_value(g_doc, path, id);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}

int
add_diskhb(
	char *id,
	char *priority
)
{
	char	path[CONF_PATH_LEN];
	char	dev_number[16];
	int		i;
	int		ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	i = atoi(id) - 300;
	i++;
	sprintf(dev_number, "%d", i);
	
	sprintf(path, "/root/heartbeat/types@diskhb");
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/heartbeat/diskhb@diskhb%s/priority", dev_number);
	ret = set_value(g_doc, path, priority);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/heartbeat/diskhb@diskhb%s/device", dev_number);
	ret = set_value(g_doc, path, id);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}

int
add_bmchbsrv(
	char* srvname,
	char* id,
	char* info
)
{
	char	path[CONF_PATH_LEN];
	int	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/server@%s/device@%s/type", srvname, id);
	ret = set_value(g_doc, path, "bmc");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/info", srvname, id);
	ret = set_value(g_doc, path, info);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_diskhbsrv(
	char* srvname, 
	char* id, 
	char* info
)
{
	char	path[CONF_PATH_LEN];
	int		ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/server@%s/device@%s/type", srvname, id);
	ret = set_value(g_doc, path, "disk");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/info", srvname, id);
	ret = set_value(g_doc, path, info);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/extend", srvname, id);
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/disk/info", srvname, id);
	ret = set_value(g_doc, path, info);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/disk/extend", srvname, id);
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}

int
add_mdc(
	char* srvname,
	char* id,
	char* info
)
{
	char	path[CONF_PATH_LEN];
	char	*dev_number[16];
	int		i;
	int		ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	i = atoi(id) - 400;
	i++;
	sprintf(dev_number, "%d", i);

	sprintf(path, "/root/server@%s/device@%s/type", srvname, id);
	ret = set_value(g_doc, path, "mdc");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/info", srvname, id);
	ret = set_value(g_doc, path, info);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/mdc/info", srvname, id);
	ret = set_value(g_doc, path, info);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}



int
add_grp(
	char *grptype,
	char *grpname
)
{
	char 	path[CONF_PATH_LEN];
	char	gid[16];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	ret = count_group();
	sprintf(gid, "%d", ret);

    if (strcmp(grptype, "failover")) {
		sprintf(path, "/root/group@%s/type", grpname);
		ret = set_value(g_doc, path, grptype);
		if (ret)
		{
			printf("set_value() failed. (ret: %d)\n", ret);
			ret = CONF_ERR_FILE;
		}
	}
	sprintf(path, "/root/group@%s/comment", grpname);
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/group@%s/gid", grpname);
	ret = set_value(g_doc, path, gid);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}

int
add_grp_param(
	char *grptype,
	char *grpname,
	char *tag,
	char *param
)
{
	char 	path[CONF_PATH_LEN];
	int		ret;
	
	/* initialize */
	ret = CONF_ERR_SUCCESS;
	sprintf(path, "/root/group@%s/%s", grpname, tag);
	ret = set_value(g_doc, path, param);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
func_exit:
	return ret;
}


int
add_rsc(
	char *grpname,
	char *rsctype,
	char *rscname
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/group@%s/resource@%s@%s", grpname, rsctype, rscname);
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/resource/types@%s", rsctype);
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/resource/%s@%s/comment", rsctype, rscname);
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_rsc_dep(
	char *rsctype,
	char *rscname,
	char *depend
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/resource/%s@%s/depend@%s", rsctype, rscname, depend);
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_rsc_param(
	char *rsctype,
	char *rscname,
	char *tag,
	char *param
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

#if 0
	sprintf(path, "/root/resource/%s@%s/parameters/%s", rsctype, rscname, tag);
#endif
	sprintf(path, "/root/resource/%s@%s/%s", rsctype, rscname, tag);
	ret = set_value(g_doc, path, param);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_rsc_guid(
	char* rsctype,
	char* rscname,
	char* srvname,
	char* tag,
	char* guid
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/resource/%s@%s/server@%s/parameters/%s", rsctype, rscname, srvname, tag);
	ret = set_value(g_doc, path, guid);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


/**
 * add_mon
 */
int
add_mon(
	char *montype,
	char *monname
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/monitor/types@%s", montype);
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/monitor/%s@%s/comment", montype, monname);
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


/**
 * clpconf_add_mon_param
 */
int
add_mon_param(
	char *montype,
	char *monname,
	char *tag,
	char *param
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/monitor/%s@%s/%s", montype, monname, tag);
	ret = set_value(g_doc, path, param);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


/**
 * clpconf_add_pingnp
 */
int
add_pingnp(
	char* priority, 
	char* device, 
	char* grpid, 
	char* listid, 
	char* ip
)
{
	char 	path[CONF_PATH_LEN];
	char	dev_number[16];
	int		i;
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	i = atoi(device) - 10200;
	i++;
	sprintf(dev_number, "%d", i);

	sprintf(path, "/root/networkpartition/types@pingnp");
	ret = set_value(g_doc, path, "");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/networkpartition/pingnp@pingnp%s/priority", dev_number);
	ret = set_value(g_doc, path, priority);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/networkpartition/pingnp@pingnp%s/device", dev_number);
	ret = set_value(g_doc, path, device);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/networkpartition/pingnp@pingnp%s/grp@%s/list@%s/ip", dev_number, grpid, listid);
	ret = set_value(g_doc, path, ip);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


int
add_pingnpsrv(
	char* srvname,
	char* id,
	char* grpid
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/server@%s/device@%s/type", srvname, id);
	ret = set_value(g_doc, path, "ping");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/info", srvname, id);
	ret = set_value(g_doc, path, grpid);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}


/**
 * clpconf_add_disknp
 */
int
add_disknp(
	char* priority, 
	char* device
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;
	char	dev_number[16];
	int		i;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	i = atoi(device) - 10100;
	i++;
	sprintf(dev_number, "%d", i);

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/networkpartition/types@disknp/disknp@disknp%s/priority", dev_number);
	ret = set_value(g_doc, path, priority);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/networkpartition/types@disknp/disknp@disknp%s/device", dev_number);
	ret = set_value(g_doc, path, device);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;

}


int
add_disknpsrv(
	char* srvname, 
	char* id, 
	char* guid, 
	char* letter
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/server@%s/device@%s/type", srvname, id);
	ret = set_value(g_doc, path, "disknp");
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/info", srvname, id);
	ret = set_value(g_doc, path, guid);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/server@%s/device@%s/extend", srvname, id);
	ret = set_value(g_doc, path, letter);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return ret;
}

int
add_cls_param(
	char *tag,
	char *param
)
{
	char 	path[CONF_PATH_LEN];
	int		ret;
	
	/* initialize */
	ret = CONF_ERR_SUCCESS;
	sprintf(path, "/root/%s", tag);
	ret = set_value(g_doc, path, param);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
func_exit:
	return ret;
}

int
add_cls_webmgr(
	char *tag,
	char *param
)
{
	char 	path[CONF_PATH_LEN];
	char	buf[CONF_PATH_LEN];
	char	*token;
	int		ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;
	memset(path, "\0", sizeof(path)); 
	memset(buf, "\0", sizeof(buf)); 

	if (!strcmp(tag, "clientlist"))
	{
		strcpy(buf, param);
		token = strchr(buf, '/');
		if (!token)
		{
			/* no subnet mask */
			strcpy(g_ip, buf);
		}
		else
		{
			strncpy(g_ip, buf, token - buf);
			g_ip[token - buf] = '\0';
			strcpy(g_subnet, token + 1);	
		}
		sprintf(path, "/root/webmgr/security/clientlist/ip@%s", g_ip);
		ret = set_value(g_doc, path, NULL);
		if (ret)
		{
			printf("set_value() failed. (ret: %d)\n", ret);
			ret = CONF_ERR_FILE;
		}
	}

func_exit:
	return ret;
}

int
add_obj_num(
	char *objnum
)
{
	int ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	set_value(g_doc, "/root/webmgr/client/objectnumber", objnum);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}

func_exit:
	return 0;
}

int
cal_obj_num()
{
	int objnum = 0;
	int srvnum = 0;
	int htbnum = 0;
	int nprnum = 0;
	int grpnum = 0;
	int rscnum = 0;
	int monnum = 0;

	/* cluster, server, group, monitor icon */
	objnum += 4;

	srvnum = count_server();
	htbnum = count_heartbeat();
	nprnum = count_networkpartition();
	grpnum = count_group();
	rscnum = count_resource();
	monnum = count_monitor();
	objnum += srvnum + grpnum + rscnum + monnum + ((htbnum + nprnum) * srvnum);

func_exit:
	return objnum;
}

int
count_hb()
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr  n;
	xmlNodePtr	n_child;
	char		element[32];
	int			ret = 0;

	root_node = xmlDocGetRootElement(g_doc);
	strcpy(element, "heartbeat");
	
	for (n = root_node->children; n != NULL; n = n->next)
	{
		if (!strcmp(n->name, element))
		{
			for (n_child = n->children; n_child != NULL; n_child = n_child->next)
			{
				if (!strcmp(n_child->name, "lanhb"))
				{
					ret++;
				}
			}
		}
	}

	return ret;
}

int
count_khb()
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr  n;
	xmlNodePtr	n_child;
	char		element[32];
	int			ret = 0;

	root_node = xmlDocGetRootElement(g_doc);
	strcpy(element, "heartbeat");
	
	for (n = root_node->children; n != NULL; n = n->next)
	{
		if (!strcmp(n->name, element))
		{
			for (n_child = n->children; n_child != NULL; n_child = n_child->next)
			{
				if (!strcmp(n_child->name, "lankhb"))
				{
					ret++;
				}
			}
		}
	}

	return ret;
}

int
count_server()
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr  n;
	char		element[32];
	int			ret = 0;

	root_node = xmlDocGetRootElement(g_doc);
	strcpy(element, "server");
	
	for (n = root_node->children; n != NULL; n = n->next)
	{
		if (!strcmp(n->name, element))
		{
			ret++;
		}
	}

	return ret;
}

int
count_group()
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr  n;
	char		element[32];
	int			ret = 0;

	root_node = xmlDocGetRootElement(g_doc);
	strcpy(element, "group");
	
	for (n = root_node->children; n != NULL; n = n->next)
	{
		if (!strcmp(n->name, element))
		{
			ret++;
		}
	}

	return ret;
}

int
count_heartbeat()
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr  n;
	xmlNodePtr	n_child;
	char		element[32];
	int			ret = 0;

	root_node = xmlDocGetRootElement(g_doc);
	strcpy(element, "heartbeat");
	
	for (n = root_node->children; n != NULL; n = n->next)
	{
		if (!strcmp(n->name, element))
		{
			for (n_child = n->children; n_child != NULL; n_child = n_child->next)
			{
				if (strcmp(n_child->name, "types") & strcmp(n_child->name, "text"))
				{
					ret++;
				}
			}
		}
	}

	return ret;
}

int
count_networkpartition()
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr  n;
	xmlNodePtr	n_child;
	char		element[32];
	int			ret = 0;

	root_node = xmlDocGetRootElement(g_doc);
	strcpy(element, "networkpartition");
	
	for (n = root_node->children; n != NULL; n = n->next)
	{
		if (!strcmp(n->name, element))
		{
			for (n_child = n->children; n_child != NULL; n_child = n_child->next)
			{
				if (strcmp(n_child->name, "types") & strcmp(n_child->name, "text"))
				{
					ret++;
				}
			}
		}
	}

	return ret;
}

int
count_resource()
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr  n;
	xmlNodePtr	n_child;
	char		element[32];
	int			ret = 0;

	root_node = xmlDocGetRootElement(g_doc);
	strcpy(element, "resource");
	
	for (n = root_node->children; n != NULL; n = n->next)
	{
		if (!strcmp(n->name, element))
		{
			for (n_child = n->children; n_child != NULL; n_child = n_child->next)
			{
				if (strcmp(n_child->name, "types") & strcmp(n_child->name, "text"))
				{
					ret++;
				}
			}
		}
	}

	return ret;
}

int
count_monitor()
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr  n;
	xmlNodePtr	n_child;
	char		element[32];
	int			ret = 0;

	root_node = xmlDocGetRootElement(g_doc);
	strcpy(element, "monitor");
	
	for (n = root_node->children; n != NULL; n = n->next)
	{
		if (!strcmp(n->name, element))
		{
			for (n_child = n->children; n_child != NULL; n_child = n_child->next)
			{
				if (strcmp(n_child->name, "types") & strcmp(n_child->name, "text"))
				{
					ret++;
				}
			}
		}
	}

	return ret;
}
