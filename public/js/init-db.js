
require("firebase");
require("firebase/firestore");
//require("firebase/auth");
//require("firebase/database");
//require("firebase/firestore");
//require("firebase/messaging");
//require("firebase/functions");

  // Initialize Firebase
  var config = {
    apiKey: "AIzaSyAx81MsPUqFY3rTBIB8QN2TuMo1ykovCLo",
    authDomain: "beer-tracker-ecaa3.firebaseapp.com",
    databaseURL: "https://beer-tracker-ecaa3.firebaseio.com",
    projectId: "beer-tracker-ecaa3",
    storageBucket: "beer-tracker-ecaa3.appspot.com",
    messagingSenderId: "974917623234"
  };

  firebase.initializeApp(config);

// Required for side-effects
//require("firebase/firestore");
//firebase.initializeApp({
//    apiKey: 'AIzaSyAx81MsPUqFY3rTBIB8QN2TuMo1ykovCLo',
//    authDomain: 'beer-tracker-ecaa3.firebaseapp.com',  // ### FIREBASE AUTH DOMAIN ###
//    projectId: 'beer-tracker-ecaa3'  // ### CLOUD FIRESTORE PROJECT ID ###
//});
