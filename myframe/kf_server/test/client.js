var WebSocket = require('/usr/local/node/lib/node_modules/ws/')
  , ws = new WebSocket('ws://127.0.0.1:9090');
  ws.on('open', function() {
      for(var i=0; i<10; i++){
          ws.send('hello world =======>');
      }
  });
  ws.on('message', function(message) {
          console.log('received: %s', message);
  });
