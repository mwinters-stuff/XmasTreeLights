const express = require('express')
const app = express()
const port = 3001

app.use(express.json());

treeInfo = {
  heap: 31496,
  ssid: "",
  ap_address: "192.168.4.1",
  pin: 2,
  leds: 10,
  current: 10,
  current_index: 10,
  current_name: "Multiple Falling Stars",
  random: true,
  override_index: -1,
  "sequences": [
      "Cylon",
      "Rocket",
      "Falling Star",
      "Rainbow",
      "Stripes",
      "Theatre Chase",
      "Fade",
      "Colour Wipe",
      "Rocket Both Ends",
      "Heartbeats",
      "Multiple Falling Stars",
      "Fire"
  ]
};

ringInfo = {
  heap: 34664,
  ssid: "",
  ap_address: "192.168.4.1",
  pin: 5,
  leds: 24,
  current: 3,
  current_index: 0,
  current_name: "Rainbow",
  random: false,
  override_index: -1,
  sequences: [
      "Rainbow",
      "Fade",
      "Colour Wipe",
      "Rocket Both Ends",
      "Heartbeats",
      "Multiple Falling Stars",
      "Fire"
  ]
};

app.get('/', function (req, res) {
  res.send('hello world')
});

app.get('/get', (req, res) => {
  console.log("getting " + req.query.what);
  if(req.query.what == "tree"){
    res.json(treeInfo);
  }else if(req.query.what == "ring"){
    res.json(ringInfo);
  }
});

app.post('/set', (req, res) => {
  console.log("setting " + req.query.what + "  " + req.body);
  if(req.query.what == "tree"){
    treeInfo.current_index = req.body.current_index;
    treeInfo.random = req.body.random;
    treeInfo.override_index = req.body.override_index;
    treeInfo.sequences = req.body.sequences;
    res.json(treeInfo);
  }else if(req.query.what == "ring"){
    ringInfo.current_index = req.body.current_index;
    ringInfo.random = req.body.random;
    ringInfo.override_index = req.body.override_index;
    ringInfo.sequences = req.body.sequences;
    
    res.json(ringInfo);
  }
});

app.listen(port, () => console.log(`Example app listening on port ${port}!`));

setInterval(() => {
  if(treeInfo.override_index === -1){
    treeInfo.current_index++;
    if(treeInfo.current_index >= treeInfo.sequences.length){
      treeInfo.current_index = 0;
    }
    treeInfo.current = treeInfo.current_index;
  }

  if(ringInfo.override_index === -1){
    ringInfo.current_index++;
    if(ringInfo.current_index >= ringInfo.sequences.length){
      ringInfo.current_index = 0;
    }
    ringInfo.current = ringInfo.current_index;
  }
  console.log(treeInfo.current_index, ringInfo.current_index);

}, 2000);