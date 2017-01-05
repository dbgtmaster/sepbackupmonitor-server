[Unit]
Description=siedlbackupmonitor-server
After=network.target

[Service]
ExecStart=/usr/local/bin/sepbackupmonitor-server
ExecReload=/bin/kill -HUP $MAINPID
KillMode=process

[Install]
WantedBy=multi-user.target
