var request = require('request');
request('http://127.0.0.1:9090/index?t=123&q=22', function (error, response, body) {
      if (!error && response.statusCode == 200) {
              console.log(body) // 打印google首页
                }
})

