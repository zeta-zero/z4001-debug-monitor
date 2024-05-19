
import "./assets/css/default_pcc.css";

import { createApp } from "vue";
import App from "./App.vue";
import { createRouter, createWebHistory } from 'vue-router';

import OverviewPage from './components/overview.vue'
import MonitorPage from './components/monitor.vue'
import ControlPage from './components/control.vue'
import InfoPage from './components/information.vue'

const routes  = [
  { path: "/", component: OverviewPage },
  { path: '/monitor', component: MonitorPage },
  { path: '/control', component: ControlPage },
  { path: '/info', component: InfoPage },
  // Add more routes as needed
];

// Create the router instance
const router  = createRouter({
  history: createWebHistory(),
  routes,
});

createApp(App)
    .use(router)
    .mount("#app");