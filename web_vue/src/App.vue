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

<style scoped>
@import url("https://cdn.jsdelivr.net/npm/bootstrap-icons@1.10.0/font/bootstrap-icons.css");

#app {
    --transition-time:0.3s;
    --secondary-transition-time:0.1s;

    background-color: var(--bsic-bg-color);
    height: 100%;
    width: 100%;
    display: flex;
    overflow:auto;
}

#app #z-sidebar {
    z-index: 10;
    background-color: var(--secondary-bg-color);
    height: 100%;
    width: 80px;
    transition: var(--transition-time);
    position: fixed;
    overflow: hidden;
}

#app #z-sidebar:hover {
    width: 180px;
}

#z-container {
    margin-top: 30px;
}

#z-sidebar .z-li {
    height: 60px;
    display: flex;
}

#z-sidebar .z-li .item-light{
    height: inherit;
    width: 8px;
    position: absolute;
    left: 0px;
    transition: var(--transition-time);
    border-top-right-radius: 8px;
    border-bottom-right-radius: 8px;
}

#z-sidebar .z-router-link {
    color: var(--primary-text-color);
    flex: 1;
    display: flex;
    border-radius: 8px;
    margin: 5px 10px 5px 18px;
    text-decoration: none;
    transition: var(--secondary-transition-time);
}

#z-sidebar .z-router-link.router-link-exact-active {
    color: var(--secondary-bg-color);
    background-color: var(--secondary-color);
}

#z-sidebar:hover .z-li .z-router-link.router-link-exact-active,
#z-sidebar .z-li:hover .item-light {
    background-color: var(--secondary-color);
}

#z-sidebar .z-li:hover .z-router-link{
    background-color: var(--primary-border-color);
}

#z-sidebar i {
    color: inherit;
    align-self: center;
    margin: 0 10px 0 10px;
    font-size: 24px;
    transition: var(--secondary-transition-time);
}

#z-sidebar .item-name {
    width: 0;
    display: flex;
    justify-content: left;
    align-items: center;
    transition: var(--secondary-transition-time);
    overflow: hidden;
    left: -10px;
    opacity: 0;
}

#z-sidebar:hover .item-name {
    color: inherit;
    width: 100%;
    display: flex;
    justify-content: left;
    align-items: center;
    left: 0;
    opacity: 1;
}

#z-sidebar .z-li:hover i,
#z-sidebar .z-li:hover .item-name{
    color: inherit;
}

#app #z-content {
    flex: 1;
    height: 100%;
    width: 100%;
    position: relative;
    margin-left: 80px;
    padding: 10px;

}
</style>