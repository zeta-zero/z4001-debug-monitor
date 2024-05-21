<template>
    <div id="info-content">
        <TInstrument ref="demoti" style="width: 100%;height: 100%;"/>
    </div>
</template>

<script setup>
import { ref } from 'vue'
import TInstrument from "./comm/Instrument.vue"

const demoti = ref(null);

window.addEventListener("load",(event)=>{
    if(demoti.value){
        const tiobj = {
            _name:"demo",
            _min:20,
            _max:80,
            _defval:40,
            _color:'#119922',
        };
        demoti.value.LoadFn(tiobj);
    }
    
});

</script>

<style scoped>
#info-content {
    width: 100%;
    height: 100%;
    background-color: white;
    padding: 10px;
    border-radius: 10px;
}
</style>