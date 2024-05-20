<template>
    <div id="content">
        <div id="led-panel" class="all-panel">
            <label class="panel-title">- LED Control</label>
            <div class="title-line"/>
            <div class="panel-body">
                <led-panel/>
            </div>
        </div>
        <div id="data-panel" class="all-panel">
            <label class="panel-title">- Data</label>
            <div class="title-line"/>
        </div>
        <div id="a-panel" class="all-panel">
            <label class="panel-title">- Data</label>
            <div class="title-line"/>
        </div>
        <div id="b-panel" class="all-panel">
            <label class="panel-title">- Data</label>
            <div class="title-line"/>
        </div>
    </div>
</template>

<script setup>
import LedPanel from "./panel/LedPanel.vue"
</script>

<style scoped>
/* Add any specific styles for Content1 here */
#content{
    min-height: 100%;
    display: grid;
    grid-template-rows: repeat(3,auto);
    grid-template-columns: repeat(4,auto);
}

.all-panel{
    background-color: var(--primary-bg-color);
    border-radius: 10px;
    padding: 20px;
    margin: 10px;
    display: flex;
    flex-direction: column;
}

.panel-title{
    color:var(--primary-text-color);
    font-weight: bold;
    padding-left:20px;
}

.panel-body{
    width: 100%;
    padding: 10px;
}

.title-line{
    width: auto;
    margin: 10px 0;
    border: 1px solid var(--primary-border-color);
}

#led-panel{
    grid-row: 1;
    grid-column: 1;
}

#data-panel{
    grid-row: 1;
    grid-column: 2;
}

#a-panel{
    grid-row: 1;
    grid-column: 3;
}

#b-panel{
    grid-row: 1;
    grid-column: 4;
}

</style>