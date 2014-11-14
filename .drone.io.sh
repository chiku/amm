#/bin/sh

set_environment() {
  export VERBOSE=1
  export CFLAGS="-m32 -Os -Wall -Wextra"
  export CXXFLAGS="-m32 -Os -Wall -Wextra"
  export LDFLAGS="-m32 -Os -Wall -Wextra"
  install_base_dir=`mktemp -d`
  install_dir="${install_base_dir}/usr/local"
  amm_binary_path="${install_dir}/bin/amm"
  current_dir="$(pwd)"
}

install_prerequisites() {
  sudo apt-get update
  sudo apt-get install g++-multilib upx
}

build() {
  ./autogen.sh
  ./configure --prefix "$install_dir"
  make clean
  make -j4
  make check -j4
  make install
}

crunch_binary() {
  reduce_binary_script=$(readlink -f $(dirname $0)/reduce.sh)
  cd "${install_dir}/bin"
  "$reduce_binary_script"
  cd "$current_dir"
}

assign_properties() {
  amm_version=$($amm_binary_path --version | cut -d' ' -f2)
  amm_size=$(ls -lah $amm_binary_path | awk '{print $5}')
  amm_package_base_dir="amm-${amm_version}-i686"
}

prepackage() {
  mkdir -p "${install_base_dir}/${amm_package_base_dir}/"
  mv "${install_base_dir}/usr" "${install_base_dir}/${amm_package_base_dir}/"
}

artifact() {
  cd "$install_base_dir"
  tar -cvzf amm.tar.gz "${amm_package_base_dir}"
  cd "$current_dir"
  cp "${install_base_dir}/amm.tar.gz" "amm-${amm_version}.tar.gz"
  cp "${install_base_dir}/${amm_package_base_dir}/usr/local/bin/amm" "amm-${amm_version}"
}

puppytize() {
  cd "$install_base_dir"
  echo "${amm_package_base_dir}|amm|${amm_version}-i686||Utility|${amm_size}||${amm_package_base_dir}.pet||Create JWM menu|Slackware|14.0||" > "${amm_package_base_dir}/pet.specs"
  tar -cvzf amm.pet "${amm_package_base_dir}"
  checksum=$(md5sum amm.pet | cut -d' ' -f1)
  echo -n "$checksum" >> amm.pet
  cd "$current_dir"
  cp $install_base_dir/amm.pet amm-$amm_version.pet
}

cleanup() {
  rm -r "$install_base_dir"
}

run() {
  commnd=$1
  echo "--- Start $commnd ---"
  "$commnd"
  echo "--- End $commnd ---"
}


set -e

run set_environment
run install_prerequisites
run build
run crunch_binary
run assign_properties
run prepackage
run artifact
run puppytize
run cleanup
