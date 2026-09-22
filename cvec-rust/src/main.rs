use std::ffi::c_void;
use std::mem::{MaybeUninit, align_of, size_of};

#[repr(C)]
struct CVec {
    data: *mut c_void,
    len: usize,
    cap: usize,
    elem_size: usize,
    elem_align: usize,
}

unsafe extern "C" {
    fn cvec_init(v: *mut CVec, elem_size: usize, elem_align: usize) -> i32;
    fn cvec_destroy(v: *mut CVec);
    fn cvec_len(v: *const CVec) -> usize;
    fn cvec_push(v: *mut CVec, elem: *const c_void) -> i32;
    fn cvec_get(v: *const CVec, index: usize) -> *const c_void;
}

fn main() {
    let mut v = MaybeUninit::<CVec>::uninit();
    let rc = unsafe { cvec_init(v.as_mut_ptr(), size_of::<i32>(), align_of::<i32>()) };
    assert_eq!(rc, 0);
    let mut v = unsafe { v.assume_init() };

    let x = 10_i32;
    let y = 20_i32;

    assert_eq!(
        unsafe { cvec_push(&mut v, (&x as *const i32).cast::<c_void>()) },
        0
    );
    assert_eq!(
        unsafe { cvec_push(&mut v, (&y as *const i32).cast::<c_void>()) },
        0
    );

    assert_eq!(unsafe { cvec_len(&v) }, 2);

    let p = unsafe { cvec_get(&v, 1) };

    assert!(!p.is_null());

    let value = unsafe { *(p.cast::<i32>()) };

    assert_eq!(value, 20);

    unsafe {
        cvec_destroy(&mut v);
    }
}
