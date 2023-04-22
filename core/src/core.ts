export type Action = void | Promise<void>

export interface AppService {
  openedFile(path: string | null): Action
  savedFileAs(path: string | null): Action
  wroteFile(path: string): Action
}

export interface HostService {
  openFile(initialDirectory: string | null): Action
  saveFileAs(initialDirectory: string | null): Action
  writeFile(path: string, text: string): Action
}

export interface ServiceCall {
  method: string
  parameters: any[]
}
