import { Backend, readFileAsString } from "@calory/frontend"
import { AppService, HostService } from "@metabolic/core"
import { createSignal, onCleanup } from "solid-js"

const host = Backend.proxy<HostService>()

export default function App() {
  const [text, setText] = createSignal("")
  const [path, setPath] = createSignal<string | null>(null)
  const [notify, setNotify] = createSignal<string | null>(null)
  
  const cancelListen = Backend.listen<AppService>({
    async openedFile(path) {
      if (path === null) return
      setPath(path)
      setText(await readFileAsString(path))
      const filename = path.substring(path.lastIndexOf("/") + 1)
      document.title = `Metabolic - ${filename}`
    },

    savedFileAs(path) {
      if (path === null) return
      setPath(path)
      const filename = path.substring(path.lastIndexOf("/") + 1)
      document.title = `Metabolic - ${filename}`
      host.writeFile(path!, text())
    },

    wroteFile(path) {
      setNotify("Saved!")
      setTimeout(() => {
        setNotify(null)
      }, 3000)
    },
  })

  onCleanup(cancelListen)

  const handleOpen = () => {
    host.openFile(null)
  }

  const handleSave = () => {
    if (path() === null) return handleSaveAs()
    host.writeFile(path()!, text())
  }

  const handleSaveAs = () => {
    host.saveFileAs(null)
  }

  return (
    <div class="layout">
      <div class="menu">
        <button onClick={handleOpen}>
          Open
        </button>
        <button onClick={handleSave}>
          Save
        </button>
        <button onClick={handleSaveAs}>
          Save As
        </button>
        {notify() && (
          <span class="notify">{notify()}</span>
        )}
      </div>
      <div class="editor">
        <textarea
          spellcheck={false}
          value={text()}
          onInput={event => setText(event.target.value)}
        ></textarea>
      </div>
    </div>
  )
}