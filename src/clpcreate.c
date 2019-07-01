#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libxml/tree.h>
#include <libxml/parser.h>

#include "clpcreate.h"

static xmlDocPtr	g_doc = NULL;
static char			*g_encoding[16];

int
main(
	int argc,
	char **argv
)
{
	xmlNodePtr	root_node = NULL;
	xmlNodePtr	new_node = NULL;
	int			ret;

	if (!strcmp(argv[1], "init"))
	{
		init(argv[2]);
		goto func_exit;
	}

	g_doc = xmlParseFile("clp.conf");
	strcpy(g_encoding, g_doc->encoding);

	if (!strcmp(argv[1], "add"))
	{
		if (!strcmp(argv[2], "cls"))
		{
			add_cls(argv[3], argv[4], argv[5]);
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
		else if (!strcmp(argv[2], "hb"))
		{
			add_hb(argv[3], argv[4]);
		}
		else if (!strcmp(argv[2], "diskhb"))
		{
			add_diskhb(argv[3], argv[4]);
		}
		else if (!strcmp(argv[2], "diskhbsrv"))
		{
			add_diskhbsrv(argv[3], argv[4], argv[5]);
		}
		else if (!strcmp(argv[2], "np"))
		{
			if (!strcmp(argv[3], "disk"))
			{
				add_disknp(argv[4], argv[5], argv[6]);
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
		else if (!strcmp(argv[2], "objnum"))
		{
			add_obj_num(argv[3]);
		}
		else
		{
		}
	}
	else
	{
		printf("invalid parameter (argv[1]: %s) \n", argv[1]);
	}

	xmlSaveFormatFileEnc("clp.conf", g_doc, g_encoding, 1);

	xmlFreeDoc(g_doc);
	xmlCleanupParser();

func_exit:
	return 0;
}


int
add_cls(
	char *clsname,
	char *lang,
	char *os
)
{
	int		ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	ret = set_value(g_doc, "/root/all/charset", lang);
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
	ret = set_value(g_doc, "/root/all/encode", lang);
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
	int			ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;
	
	sprintf(path, "/root/server@%s/device@%s/type", srvname, id);
	ret = set_value(g_doc, path, "lan");
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

	i = atoi(id);
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
add_grp(
	char *grptype,
	char *grpname
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/group@%s/type", grpname);
	ret = set_value(g_doc, path, grptype);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/group@%s/comment", grpname);
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
	char* npname, 
	char* priority, 
	char* device
)
{
	char 	path[CONF_PATH_LEN];
	int 	ret;

	/* initialize */
	ret = CONF_ERR_SUCCESS;

	sprintf(path, "/root/networkpartition/types@disknp/disknp@%s/priority", npname);
	ret = set_value(g_doc, path, priority);
	if (ret)
	{
		printf("set_value() failed. (ret: %d)\n", ret);
		ret = CONF_ERR_FILE;
	}
	sprintf(path, "/root/networkpartition/types@disknp/disknp@%s/device", npname);
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
