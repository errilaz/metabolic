run:
  @# @cd ../calory/bridge && pnpm build
  @just -d host -f host/justfile build
  @# @just -d ../calory/launcher -f ../calory/launcher/justfile build
  @export CALORY_ENV=development && \
    ../calory/launcher/bin/calory

bundletest:
  # cd ../calory/bridge && pnpm build
  # just -d host -f host/justfile build
  # just -d ../calory/launcher -f ../calory/launcher/justfile build
  rm -rf build
  mkdir -p build/app/dist
  cp app/dist/index.html build/app/dist/index.html
  cp ../calory/launcher/bin/calory build/calory

