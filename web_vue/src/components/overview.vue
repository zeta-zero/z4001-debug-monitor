<template>
    <div id="content">
        <div id="led-panel" class="all-panel">
            <label class="panel-title">- LED Control</label>
            <div class="title-line" />
            <div class="panel-body">
                <led-panel />
            </div>
        </div>
        <div id="data-panel" class="all-panel">
            <label class="panel-title">- Data</label>
            <div class="title-line" />
            <div class="panel-body">
                <TInstrument ref="demoti" />
            </div>
        </div>
        <div id="a-panel" class="all-panel">
            <label class="panel-title">- Data</label>
            <div class="title-line" />
            <div class="panel-body">
                <TLineChart ref="demotlc" />
            </div>
        </div>
        <div id="b-panel" class="all-panel">
            <label class="panel-title">- Data</label>
            <div class="title-line" />
        </div>
    </div>
</template>

<script setup>
import { ref, onMounted, onUpdated } from 'vue'
import LedPanel from "./panel/LedPanel.vue"
import TInstrument from "./comm/Instrument.vue"
import TLineChart from "./comm/LineChart.vue"

const demoti = ref(null);
const demotlc = ref(null);

onMounted(() => {
    if (demotlc.value) {
        demotlc.value.LoadFn("Demo2");
    }
    if (demoti.value) {
        const tiobj = {
            _name: "demo",
            _min: 20,
            _max: 80,
            _defval: 40,
            _color: '#58D9F9',
        };
        demoti.value.LoadFn(tiobj);
    }
    console.log("onMounted");
})

onUpdated(() => {

    console.log("update");
})

setInterval(function () {

    if (demotlc.value) {
        demotlc.value.Update("line1", Math.random() * 10 + 10);
        demotlc.value.Update("line2", Math.random() * 10 + 10);
    }
    if (demoti.value) {
        demoti.value.Update( Math.random() * 50 + 20);
    }

}, 500);

</script>

<style scoped>
/* Add any specific styles for Content1 here */
#content {
    min-height: 100%;
    display: grid;
    grid-template-rows: repeat(3, 40%);
    grid-template-columns: repeat(4, 25%);
}

.all-panel {
    background-color: var(--primary-bg-color);
    border-radius: 10px;
    padding: 20px;
    margin: 10px;
    display: flex;
    flex-direction: column;
}

.panel-title {
    color: var(--primary-text-color);
    font-weight: bold;
    padding-left: 20px;
}

.panel-body {
    width: 100%;
    height: 100%;
    padding: 10px;
    display: inline;
}

.title-line {
    width: auto;
    margin: 10px 0;
    border: 1px solid var(--primary-border-color);
}

#led-panel {
    grid-row: 1;
    grid-column: 1;
}

#data-panel {
    grid-row: 1;
    grid-column: 2;
}

#a-panel {
    grid-row: 1;
    grid-column: 3;
}

#b-panel {
    grid-row: 1;
    grid-column: 4;
}
</style>