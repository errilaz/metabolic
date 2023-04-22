import { Backend } from "@calory/frontend"

export default function App() {
  function receive(message: any) {
    console.log(message)
  }

  Backend.addMessageListener(receive)

  return (
    <div>
      hello
      <div>
        <button onClick={event => {
          window.postMessage({
            __envelope_to: "host",
            message: {
              method: "openFile",
              parameters: [null],
            }
          })        
        }}>Open File</button>
      </div>
    </div>
  )
}