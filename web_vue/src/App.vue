<template>
    <div id="app">
        <div id="z-sidebar">
            <div id="z-container">

                <div class="z-li">
                    <div class="item-light"/>
                    <router-link to="/" class="z-router-link">
                        <i class="bi bi-house-door z-baritem-setting"></i>
                        <span class="item-name">Overview</span>
                    </router-link>
                </div>
                <div class="z-li">
                    <div class="item-light"/>
                    <router-link class="z-router-link" to="/monitor">
                        <i class="bi bi-book z-baritem-setting"></i>
                        <div class="item-name">Monitor</div>
                    </router-link>
                </div>
                <div class="z-li">
                    <div class="item-light"/>
                    <router-link class="z-router-link" to="/control">
                        <i class="bi bi-cup-straw z-baritem-setting"></i>
                        <div class="item-name">Control</div>
                    </router-link>
                </div>
                <div class="z-li">
                    <div class="item-light"/>
                    <router-link class="z-router-link" to="/info">
                        <i class="bi bi-person z-baritem-setting"></i>
                        <div class="item-name">Information</div>
                    </router-link>
                </div>
            </div>
        </div>

        <div id="z-content">
            <router-view/>
        </div>
    </div>
</template>

<script setup>
</script>

<style>
@import url("https://cdn.jsdelivr.net/npm/bootstrap-icons@1.10.0/font/bootstrap-icons.css");

#app {
    background-color: lightgray;
    height: 100%;
    display: flex;
    position: relative;
}

#app #z-sidebar {
    justify-content: left;
    background-color: darkolivegreen;
    left: 0;
    height: 100%;
    width: 80px;
    transition: 0.3s;
    overflow: hidden;
}

#app #z-sidebar:hover {
    width: 180px;
}

#z-container {
    width: 100%;
    padding-top: 30px;
}

#z-sidebar .z-li {
    height: 60px;
    padding: 0 10px 0 18px;
}

#z-sidebar .z-li .item-light{
    height: 48px;
    width: 4px;
    position: absolute;
    left: 0px;
    transition: 0.6s;
    border-top-right-radius: 4px;
    border-bottom-right-radius: 4px;
}

#z-sidebar .z-router-link {
    color: whitesmoke;
    height: 80%;
    display: flex;
    border-radius: 8px;
    text-decoration: none;
    transition: 0.6s;
}

#z-sidebar .z-router-link.router-link-exact-active {
    color: lightblue;
}

#z-sidebar:hover .z-li .z-router-link.router-link-exact-active {
    background-color: chocolate;
}

#z-sidebar .z-li:hover .item-light{
    background-color: orange;
}

#z-sidebar .z-li:hover .z-router-link{
    background-color: lightgray;
}

#z-sidebar i {
    color: inherit;
    align-self: center;
    padding: 0px 10px 0 10px;
    font-size: 24px;
    transition: 0.3s;
}

#z-sidebar .item-name {
    width: 0;
    display: flex;
    justify-content: left;
    align-items: center;
    transition: 0.3s;
    overflow: hidden;
    position: relative;
    left: -10px;
    opacity: 0;
}

#z-sidebar:hover .item-name {
    color: inherit;
    width: 100%;
    display: flex;
    justify-content: left;
    align-items: center;
    position: relative;
    left: 0;
    opacity: 1;
}

#z-sidebar .z-li:hover i,
#z-sidebar .z-li:hover .item-name{
    color: black;
}

#app #z-content {
    height: 100%;
    flex: 1;
    padding: 20px;
    background-color: cornflowerblue;
}
</style>