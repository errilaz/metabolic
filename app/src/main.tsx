/* @refresh reload */
import { render } from "solid-js/web"
import "./theme.css"

import App from "./App"

const root = document.getElementById("root")

render(() => <App />, root!)
