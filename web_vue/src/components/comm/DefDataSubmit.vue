<template>
    <div id="localgrid">
        <label id="labelname">{{ name }}</label>
        <input id="localinput" type="text"></input>
        <button class="btn btn-primary" id="localbtn" @click="BtnProcEvent">{{ btnname }}</button>
    </div>
</template>

<script setup>
import {ref} from 'vue'
import { defineProps,defineEmits } from 'vue'

const localProps = defineProps({
    name: ref("name"),
    btnname: ref("button")
});

const localEmits = defineEmits([
    'onclick',
]);

function BtnProcEvent(){
    localEmits('onclick');
}
</script>

<style scoped>
#localgrid{
    display: grid;
    grid-template-columns: 20% 50% 30%;
}

#localgrid #labelname{
    grid-column: 1;
    align-content: center;
}

#localgrid #localinput{
    grid-column: 2;
    border-color: var(--primary-color);
    background-color:bl;
    border-radius: 4px 0 0 4px;
    border-width: 1px 0 1px 1px;
}

#localgrid #localinput:focus{
    outline: none;
}

#localgrid #localbtn{
    grid-column: 3;
    border-radius: 0 4px 4px 0;
    border-width: 1px 1px 1px 0;
}

</style>