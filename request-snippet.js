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
                beforeSend: function(request) {
                 request.setRequestHeader("Authorization", 'key ttn-account-v2.roDff_0RLN8RalPCctiau1q-gLAtb-QhzTy3lO4EtfY');
                 request.setRequestHeader("Accept", "application/json");
                },
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