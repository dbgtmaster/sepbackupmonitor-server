[Unit]
Description=siedlbackupmonitor-server
After=network.target

[Service]
ExecStart=/usr/local/bin/siedlbackupmonitor-server
ExecReload=/bin/kill -HUP $MAINPID
KillMode=process

[Install]
WantedBy=multi-user.target