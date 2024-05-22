<template>
    <div id="lc-content">
        <div id="echarts-content" />
    </div>
</template>

<script setup>
import { defineExpose } from 'vue';

let LocalChart;
const Option = {
    title: {
        show: true,
        text: 'demo',
    },
    tooltip: {
        trigger: 'axis',
        show: true,
        axisPointer: {
            animation: false,
        },
    },
    xAxis: {
        type: 'category',
        min: 0,
        max: function (value) {
            return value < 10 ? 10 : Math.floor(value.max) + 10;
        },
    },
    yAxis: {
        type: 'value',
        min: 0,
        max: function (value) {
            return value < 10 ? 10 : Math.floor(value.max) + 1;
        },
    },
    legend: {
        data: [],
    },
    series: [],
};

const DataMap = new Map();

function LoadFn(_name) {

    const chartDom = document.getElementById('echarts-content');
    if (!isNaN(LocalChart)) {
        LocalChart.dispose();
    }
    if (typeof (_name) == "string") {
        Option.title.text = _name;
    }
    LocalChart = echarts.init(chartDom);
    LocalChart.setOption(Option);
}

function Update(_name, _val, _color) {
    if (!LocalChart && typeof (LocalChart) == 'undefined') {
        return;
    }
    if (typeof (_name) != 'string' || (!Array.isArray(_val) && typeof (_val) != 'number')) {
        return;
    }
    if (DataMap.has(_name) == false) {
        DataMap.set(_name, { data: _val, color: _color });
        Option.legend.data.push(_name);
        if (typeof (_val) == 'number') {
            Option.series.push({
                name: _name,
                type: 'line',
                data: [_val],
                showSymbol: false,

            });
        }
        else {
            Option.series.push({
                name: _name,
                type: 'line',
                data: _val,
                showSymbol: false,

            });
        }

    }
    else {
        let i = 0;
        for (i = 0; i < Option.series.length; i++) {
            if (Option.series[i].name == _name) {
                if (typeof (_val) == 'number') {
                    Option.series[i].data.push(_val);
                }
                else {
                    Option.series[i].data.push.apply(Option.series[i].data, _val);
                }
                break;
            }
        }
    }

    LocalChart.setOption(Option);
}

// valss [{name,val}]
function UpdateMore(vals) {
    vals.forEach(item => {
        if (DataMap.has(item.name) == false) {
            DataMap.set(item.name, { data: item.val, color: item.color });
            Option.legend.data.push(item.name);
            if (typeof (item.val) == 'number') {
                Option.series.push({
                    name: item.name,
                    type: 'line',
                    data: [item.val],
                    showSymbol: false,

                });
            }
            else {
                Option.series.push({
                    name: item.name,
                    type: 'line',
                    data: item.val,
                    showSymbol: false,

                });
            }

        }
        else {
            let i = 0;
            for (i = 0; i < Option.series.length; i++) {
                if (Option.series[i].name == item.name) {
                    if (typeof (item.val) == 'number') {
                        Option.series[i].data.push(item.val);
                    }
                    else {
                        Option.series[i].data.push.apply(Option.series[i].data, item.val);
                    }
                    break;
                }
            }
        }
    });
    LocalChart.setOption(Option);
}

defineExpose({
    LoadFn,
    Update,
    UpdateMore,
});

</script>

<style scoped>
#lc-content,
#lc-content #echarts-content {
    width: 100%;
    height: 100%;
}
</style>
