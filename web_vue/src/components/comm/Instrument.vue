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
        splitNumber: 10,
        animation: true,
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
            icon: 'path://M2090.36389,615.30999 L2090.36389,615.30999 C2091.48372,615.30999 2092.40383,616.194028 2092.44859,617.312956 L2096.90698,728.755929 C2097.05155,732.369577 2094.2393,735.416212 2090.62566,735.56078 C2090.53845,735.564269 2090.45117,735.566014 2090.36389,735.566014 L2090.36389,735.566014 C2086.74736,735.566014 2083.81557,732.63423 2083.81557,729.017692 C2083.81557,728.930412 2083.81732,728.84314 2083.82081,728.755929 L2088.2792,617.312956 C2088.32396,616.194028 2089.24407,615.30999 2090.36389,615.30999 Z',
            width: 16,
            length: '70%',
        },
        title: {
            show: false
        },

        detail: {
            formatter: function (value) {
                return value.toFixed(2)
            },
            offsetCenter: [0, '30%'],
        },
        data: [
            {
                value: 0,
                name: "value",
                detail: {
                    valueAnimation: true,
                    offsetCenter: ['0%', '0%']
                }
            },
            {
                value:100,
                name:"test",
                detail: {
                    valueAnimation: true,
                    offsetCenter: ['0%', '40%']
                }
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
    // if (typeof (val._color) == "string" && val._color != "") {
    //     Option.series[0].itemStyle.color = val._color;
    // }
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
                {value: 100 - _val,},
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
