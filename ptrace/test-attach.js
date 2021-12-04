const http = require("http");
http.createServer((req, res) => res.end('ok')).listen(9999);
setInterval(() => {
	http.get('http://localhost:9999');
}, 5000);
