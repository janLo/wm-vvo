#
# Regular cron jobs for the wm-vvo package
#
0 4	* * *	root	[ -x /usr/bin/wm-vvo_maintenance ] && /usr/bin/wm-vvo_maintenance
