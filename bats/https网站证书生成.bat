@rem 生成私钥文件
openssl genrsa -out private.key 1024

openssl req -new -key private.key -out cert_req.csr

@rem 指定密钥密钥文件来生成一个ca请求
@rem 这个步骤会要求填入国家区域以及域名等信息
@rem 最重要的一行是Common Name，需要填入与服务器关联的域名，或者是您服务器的公共IP地址

@rem 将请求发给 CA 机构进行签名
openssl x509 -req -days 365 -in cert_req.csr -signkey private.key -out server_cert.crt