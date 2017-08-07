var WebSocketServer = require('/usr/local/node/lib/node_modules/ws/').Server
   , wss = new WebSocketServer({port: 9090});
   wss.on('connection', function(ws) {
           ws.on('message', function(message) {
                       console.log('received: %s', message);
                           });
               ws.send('something');
   });
