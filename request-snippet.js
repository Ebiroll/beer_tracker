

                $.ajax({
                    // https://github.com/mapshakers/leaflet-icon-pulse
                    url: 'https://esp32_heltec.data.thethingsnetwork.org/api/v2/query?last=2d',
                    method: "GET",
                    //url: 'beer-tracker-ecaa3/us-central1/helloWorld',
                    //url: 'https://us-central1-beer-tracker-ecaa3.cloudfunctions.net/helloWorld',
                    headers: { "Authorization" : 'key ttn-account-v2.roDff_0RLN8RalPCctiau1q-gLAtb-QhzTy3lO4EtfY' },
                    dataType : "jsonp",
                    local : false,
                    success: function(data){
                        console.log("XXXXXXXXXXX",data);
                        var out = [];
                        for(single in data) {
                         //console.log(data[single].lat);
                         if (data[single].lon && data[single].lat) {
                            var tobj =  { 
                            type: "Feature", 
                            properties: { description: "Single channel dragino raspi", id: "olle" }, 
                            geometry: { type: "Point", coordinates: [data[single].lon , data[single].lat , 0.0 ] } };
                          L.geoJSON(tobj).addTo(map);
                         }
                         }
                    },
                    error: function(XMLHttpRequest, textStatus, errorThrown) { 
                        console.log("Status: " + textStatus);
                        console.log("Error: " + errorThrown); 
                    }       
                    });
    

// https://stackoverflow.com/questions/5485495/how-can-i-take-advantage-of-callback-functions-for-asynchronous-xmlhttprequest

 
            function reqListener () {
                console.log(this.response);
               //L.geoJson(JSON.parse(this.response)).addTo(map);
             }

            var oReq = new XMLHttpRequest();
            oReq.addEventListener("load", reqListener);
            window.onload = function(e){ 
                console.log("window.onload" );
                //oReq.open("GET", "https://esp32_heltec.data.thethingsnetwork.org/api/v2/query?last=2d");
                oReq.open("GET", "data.json");
                oReq.setRequestHeader('Authorization', 'key ttn-account-v2.roDff_0RLN8RalPCctiau1q-gLAtb-QhzTy3lO4EtfY');
                oReq.send();
            }


            $.ajax({
                //url: 'https://esp32_heltec.data.thethingsnetwork.org/api/v2/query?last=2d',
                url: 'data.json',
                dataType : "json",
                method: "GET",
                beforeSend : function(request) {
                    request.setRequestHeader('Authorization' , 'key ttn-account-v2.roDff_0RLN8RalPCctiau1q-gLAtb-QhzTy3lO4EtfY');
                },
                // One of those ...
                headers: { "Authorization" : 'key ttn-account-v2.roDff_0RLN8RalPCctiau1q-gLAtb-QhzTy3lO4EtfY' },
                success: function(dataWeGotViaJsonp){
                    var text = '';
                    var len = dataWeGotViaJsonp.length;
                    console.log("XXXXXXXXXXX",dataWeGotViaJsonp);
                },
                error: function(XMLHttpRequest, textStatus, errorThrown) { 
                    alert("Status: " + textStatus);
                    alert("Error: " + errorThrown); 
                }       
            });


            var aut = {Authorization : 'key ttn-account-v2.roDff_0RLN8RalPCctiau1q-gLAtb-QhzTy3lO4EtfY'};
            var geojsonLayer = L.geoJson.ajax("data.json", {
                headers: {'Authorization' : 'key ttn-account-v2.roDff_0RLN8RalPCctiau1q-gLAtb-QhzTy3lO4EtfY'},
                local: false,
                middleware:function(data){
                    var out = [];
                    for(single in data) {
                     console.log(data[single].lat);
                     var tobj =  { 
                      type: "Feature", 
                      properties: { description: "Single channel dragino raspi", id: "olle" }, 
                      geometry: { type: "Point", coordinates: [data[single].lon , data[single].lat , 0.0 ] } };
                      out.push(tobj);
                     }
                     return out;
                }
            });
            map.addLayer(geojsonLayer);