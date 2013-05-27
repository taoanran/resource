ssh-keygen -t rsa  生成密钥
把客户机的id_rsa.pub上传到git server 上keydir目录，改名为name.pub, chmod 777 *， 然后再在gitosis.conf中添加Member即可。