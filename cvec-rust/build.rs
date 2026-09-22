fn main() {
    let dst = cmake::Config::new("../cvec").build();
    println!("cargo:rustc-link-search=native={}/lib", dst.display());
    println!("cargo:rustc-link-lib=static=cvec");
}
