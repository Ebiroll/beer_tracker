const functions = require("firebase-functions");

// Create and Deploy Your First Cloud Functions
// https://firebase.google.com/docs/functions/write-firebase-functions

exports.helloWorld = functions.https.onRequest((request, response) => {
  // var term = request.query.q;
  // See https://howtofirebase.com/firebase-cloud-functions-753935e80323
  // why we're requiring it inside the function
  //  require("./secret")
  var TT_URL = "https://esp32_heltec.data.thethingsnetwork.org/api/v2/query?last=2d";
  const axios = require("axios");

  //const instance = axios.create({
  //  baseURL: 'https://some-domain.com/api/',
  //  timeout: 1000,
  //  headers: {'X-Custom-Header': 'foobar'}
  //});

  axios
    .get(TT_URL, {
      params: {
        type: "json",
        last:"2d",        
      }, 
      headers: {
        'Authorization':'key ttn-account-v2.roDff_0RLN8RalPCctiau1q-gLAtb-QhzTy3lO4EtfY',
        'Content-Type':'application/json'
      },
    })
    .then(TTNData => {
      response.set("Access-Control-Allow-Origin", "*");
      response.set("Access-Control-Allow-Methods", "GET, POST");
      response.status(200).send(TTNData.data.items);
    })
    .catch(err => console.error(err));
});
