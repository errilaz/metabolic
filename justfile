run:
  @cd ../calory/bridge && pnpm build
  @just -d host -f host/justfile build
  @just -d ../calory/launcher -f ../calory/launcher/justfile build
  @export CALORY_ENV=development && \
    ../calory/launcher/bin/calory
