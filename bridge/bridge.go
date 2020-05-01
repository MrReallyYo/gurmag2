package bridge

/*
#cgo windows LDFLAGS: -lgdi32 -luser32
*/
// #include "bridge.h"
import "C"
import (
	"bytes"
	"fmt"
	"image"
	"image/color"
	"image/png"
	"os"
	"unsafe"
)

func GetScreen(name string, clazz string) (image.Rectangle, []byte) {
	screen := C.GetScreen(C.CString(name), C.CString(clazz))

	rect := image.Rectangle{
		Min: image.Point{X: int(screen.left), Y: int(screen.top)},
		Max: image.Point{X: int(screen.right), Y: int(screen.bottom)},
	}
	rgbPtr := unsafe.Pointer(screen.buffer.ptr)
	rgbBuffer := C.GoBytes(rgbPtr, screen.buffer.size)
	C.free(rgbPtr)
	return rect, rgbBuffer
}

func GetMousePos() (int, int) {
	pos := C.GetMousePos()
	return int(pos.x), int(pos.y)
}

func Dump(rect image.Rectangle, rgbBuffer []byte) {
	fmt.Println("Buffer:", len(rgbBuffer))

	out, err := os.Create("./bla.png")
	fmt.Println(err)

	w := rect.Max.X - rect.Min.X
	h := rect.Max.Y - rect.Min.Y

	m0 := image.NewRGBA(image.Rect(0, 0, w, h))
	for y := 0; y < h; y++ {
		for x := 0; x < w; x++ {
			pixel := x + y*w
			r := rgbBuffer[3*pixel]
			g := rgbBuffer[3*pixel+1]
			b := rgbBuffer[3*pixel+2]
			m0.Set(x, y, color.RGBA{R: r, G: g, B: b, A: 255})
		}
	}

	fmt.Println(err)
	_, _, err = image.Decode(bytes.NewReader(rgbBuffer))
	fmt.Println(err)

	err = png.Encode(out, m0)
	fmt.Println(err)
}
