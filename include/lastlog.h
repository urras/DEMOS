#ifndef _lastlog_h_
#       define  _lastlog_h_

struct lastlog {
	time_t  ll_time;
	char    ll_line[8];
#ifdef  UCB_NET
	char    ll_host[16];            /* same as in utmp */
#endif
};

#endif _lastlog_h_
