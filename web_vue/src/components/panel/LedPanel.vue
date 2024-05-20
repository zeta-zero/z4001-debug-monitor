<template>
    <div id="ledp-content">
        <def-data-submit name="LED RGB" btnname="Set" :inputmaxlen="6"
                        inputmatchfn="this.value = this.value.replace(/[^0-9A-Fa-f]/g, '')"
                        @onclick="ledBtnProcEvent"/>
    </div>
</template>

<script setup>
import DefDataSubmit from "../comm/DefDataSubmit.vue";
import {GP_PackData,GP_Code} from "../../assets/js/zCommGP.js";

function ledBtnProcEvent(strnum){
    const number = strnum;
    if (number === "" || typeof(number) != "string") {
        alert("Please enter a valid number");
        return;
    }

    const encdata = GP_PackData(GP_Code.SetLEDColor,strnum);

    // Create an XMLHttpRequest object
    const xhr = new XMLHttpRequest();
    // const url = 'http://192.168.0.104'; // Replace with your server URL
    const url = window.location.origin;

    xhr.open("POST", url, true);
    xhr.setRequestHeader("Content-Type", "text/plain");

    // Define what happens on successful data submission
    xhr.onreadystatechange = function () {
        if (xhr.readyState === 4 && xhr.status === 200) {
            console.log('Response from server: ', xhr.responseText);
        }
    };

    // Define what happens in case of an error
    xhr.onerror = function (e) {
        console.error('Error while sending encoded number',e);
    };

    // Send the request with the encoded number
    xhr.send(encdata);
}
</script>

<style scoped>


</style>
