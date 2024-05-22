<template>
    <div id="inst-content">
        <div id="ECharts-Content"></div>
    </div>
</template>

<script setup>
import { defineExpose } from 'vue';

let LocalChart;
const Option = {
    series: [{
        type: 'gauge',
        min: 0,
        max: 100,
        radius: '100%',
        splitNumber: 5,
        animation: true,
        itemStyle:{
            color:'#117aFF',
        },
        axisLine: {
            roundCap: true,
            lineStyle: {
                width: 18,
            }
        },
        splitLine: {
            show: false,
            distance: 0,
            length: 10
        },
        axisTick: {
            splitNumber: 2,
            lineStyle: {
                width: 2,
                color: '#999'
            }
        },
        axisLabel: {
            distance: 40,
            color: '#999',
            fontSize: 20
        },
        axisLine: {
            lineStyle: {
            width: 20
            }
        },
        splitLine: {
            length: 10,
            distance:0,
            lineStyle: {
                width: 3,
                color: '#999'
            }
        },
        progress: {
            show: true,
            roundCap: true,
            overlap: false,
            width: 10,
        },
        pointer: {
            show:false,
        },
        title: {
            show: false
        },

        detail: {
            formatter: function (value) {
                return value.toFixed(2)
            },
            color:'#343A40',
            offsetCenter: [0, '70%'],
        },
        data: [
            {
                value: 0,
                name: "value",
            },
        ]
    }],
};

function LoadFn(val = { _name, _min, _max, _defval, _color }) {

    const chartDom = document.getElementById('ECharts-Content');
    if (!isNaN(LocalChart)) {
        LocalChart.dispose();
    }
    if (!isNaN(val._name)) {
        Option.series[0].name = val._name;
    }
    if (!isNaN(val._min)) {
        Option.series[0].min = val._min;
    }
    if (!isNaN(val._max)) {
        Option.series[0].max = val._max;
    }
    if (!isNaN(val._defval)) {
        Option.series[0].data[0].value = val._defval;
    }
    if (typeof (val._color) == "string" && val._color != "") {
        Option.series[0].itemStyle.color = val._color;
    }
    LocalChart = echarts.init(chartDom);
    LocalChart.setOption(Option);
}

function Update(_val) {
    if (typeof (_val) != 'number') {
        return;
    }
    LocalChart.setOption({
        series: [{
            data: [
                {value: _val,},
            ],
        }],
    });
}

defineExpose({
    LoadFn,
    Update,
});

</script>

<style scoped>
#inst-content,
#inst-content #ECharts-Content {
    width: 100%;
    height: 100%;
}
</style>
