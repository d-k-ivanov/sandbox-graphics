# Some Formulas

* [Online TeX to Image converter](http://www.sciweavers.org/free-online-latex-equation-editor)

## Ray - Camera

### No Text blocks

```tex
ray\big(t\big)  = camera + \frac{pixel - camera}{\|pixel - camera\|} \times t
```

![ray](https://render.githubusercontent.com/render/math?math=ray%5Cbig%28t%5Cbig%29%20%20%3D%20camera%20%2B%20%5Cfrac%7Bpixel%20-%20camera%7D%7B%5C%7Cpixel%20-%20camera%5C%7C%7D%20%5Ctimes%20t&mode=inline)

---

### With text for TeX

```tex
\text{ray}\big(t\big) = \text{camera} + \frac{\text{pixel} - \text{camera}}{\|\text{pixel} - \text{camera}\|} \times \text{t}
```

![ray](https://render.githubusercontent.com/render/math?math=%5Ctext%7Bray%7D%5Cbig%28t%5Cbig%29%20%3D%20%5Ctext%7Bcamera%7D%20%2B%20%5Cfrac%7B%5Ctext%7Bpixel%7D%20-%20%5Ctext%7Bcamera%7D%7D%7B%5C%7C%5Ctext%7Bpixel%7D%20-%20%5Ctext%7Bcamera%7D%5C%7C%7D%20%5Ctimes%20%5Ctext%7Bt%7D)

---

### Large font

```tex
{\Large\text{ray}\big(t\big) = \text{camera} + \frac{\text{pixel} - \text{camera}}{\|\text{pixel} - \text{camera}\|} \times \text{t}}
```

![ray](https://render.githubusercontent.com/render/math?math=%7B%5CLarge%5Ctext%7Bray%7D%5Cbig%28t%5Cbig%29%20%3D%20%5Ctext%7Bcamera%7D%20%2B%20%5Cfrac%7B%5Ctext%7Bpixel%7D%20-%20%5Ctext%7Bcamera%7D%7D%7B%5C%7C%5Ctext%7Bpixel%7D%20-%20%5Ctext%7Bcamera%7D%5C%7C%7D%20%5Ctimes%20%5Ctext%7Bt%7D%7D&mode=inline)

---

### Huge font

```tex
{\Huge\text{ray}\big(t\big) = \text{camera} + \frac{\text{pixel} - \text{camera}}{\|\text{pixel} - \text{camera}\|} \times \text{t}}
```

![ray](https://render.githubusercontent.com/render/math?math=%7B%5CHuge%5Ctext%7Bray%7D%5Cbig%28t%5Cbig%29%20%3D%20%5Ctext%7Bcamera%7D%20%2B%20%5Cfrac%7B%5Ctext%7Bpixel%7D%20-%20%5Ctext%7Bcamera%7D%7D%7B%5C%7C%5Ctext%7Bpixel%7D%20-%20%5Ctext%7Bcamera%7D%5C%7C%7D%20%5Ctimes%20%5Ctext%7Bt%7D%7D&mode=inline)

---

### Plain text

```txt
                        pixel  -  camera
ray(t)  =  camera  +  -------------------- t
                      ||pixel  -  camera||
```

## Ray - Origin

```tex
{\Large ray\big(t\big) = O + \frac{D - O}{\lVert D - O \rVert} \cdot t = O + d \cdot t}
```

![ray](https://render.githubusercontent.com/render/math?math=%7B%5CLarge%20ray%5Cbig%28t%5Cbig%29%20%3D%20O%20%2B%20%5Cfrac%7BD%20-%20O%7D%7B%5ClVert%20D%20-%20O%20%5CrVert%7D%20%5Ccdot%20t%20%3D%20O%20%2B%20d%20%5Ccdot%20t%7D&mode=inline)

---

## Sphere

### Equation

```tex
{\Large \lVert X - C \rVert = r}
```

![sphere](https://render.githubusercontent.com/render/math?math=%7B%5CLarge%20%5ClVert%20X%20-%20C%20%5CrVert%20%3D%20r%7D&mode=inline)

### Intersection

```tex
{\Large \lVert X - C \rVert^{2} = r^{2}}
```

![sphere_intersection](https://render.githubusercontent.com/render/math?math=%7B%5CLarge%20%5ClVert%20X%20-%20C%20%5CrVert%5E%7B2%7D%20%3D%20r%5E%7B2%7D%7D&mode=inline)

```tex
\begin{align*}
\\{\Large \lVert ray\big(t\big) - C \rVert^{2} = r^{2}}
\\{\Large \lVert O + d \cdot t - C \rVert^{2} = r^{2}}
\\{\Large \langle d \cdot t + O - C, d \cdot t + O - C \rangle = r^{2}}
\\{\Large \langle d,d \rangle t^{2} + 2t \langle d,O - C \rangle + \langle O - C,O - C \rangle = r^{2}}
\\{\Large \langle d,d \rangle t^{2} + 2t \langle d,O - C \rangle + \lVert O - C \rVert^{2} - r^{2} = 0}
\end{align*}
```

![sphere_intersection](https://render.githubusercontent.com/render/math?math=%5Cbegin%7Balign%2A%7D%0A%5C%5C%7B%5CLarge%20%5ClVert%20ray%5Cbig%28t%5Cbig%29%20-%20C%20%5CrVert%5E%7B2%7D%20%3D%20r%5E%7B2%7D%7D%0A%5C%5C%7B%5CLarge%20%5ClVert%20O%20%2B%20d%20%5Ccdot%20t%20-%20C%20%5CrVert%5E%7B2%7D%20%3D%20r%5E%7B2%7D%7D%0A%5C%5C%7B%5CLarge%20%5Clangle%20d%20%5Ccdot%20t%20%2B%20O%20-%20C%2C%20d%20%5Ccdot%20t%20%2B%20O%20-%20C%20%5Crangle%20%3D%20r%5E%7B2%7D%7D%0A%5C%5C%7B%5CLarge%20%5Clangle%20d%2Cd%20%5Crangle%20t%5E%7B2%7D%20%2B%202t%20%5Clangle%20d%2CO%20-%20C%20%5Crangle%20%2B%20%5Clangle%20O%20-%20C%2CO%20-%20C%20%5Crangle%20%3D%20r%5E%7B2%7D%7D%0A%5C%5C%7B%5CLarge%20%5Clangle%20d%2Cd%20%5Crangle%20t%5E%7B2%7D%20%2B%202t%20%5Clangle%20d%2CO%20-%20C%20%5Crangle%20%2B%20%5ClVert%20O%20-%20C%20%5CrVert%5E%7B2%7D%20-%20r%5E%7B2%7D%20%3D%200%7D%0A%5Cend%7Balign%2A%7D&mode=inline)

```tex
\begin{align*}
\\{\Large a = \lVert d \rVert^{2} = 1}
\\{\Large b = 2\langle d,O - C \rangle}
\\{\Large c = \lVert O - C \rVert^{2} - r^{2}}
\\{\Large \Delta = b^{2} - 4ac}
\end{align*}
```

![quadratic_equation](https://render.githubusercontent.com/render/math?math=%5Cbegin%7Balign%2A%7D%0A%5C%5C%7B%5CLarge%20a%20%3D%20%5ClVert%20d%20%5CrVert%5E%7B2%7D%20%3D%201%7D%0A%5C%5C%7B%5CLarge%20b%20%3D%202%5Clangle%20d%2CO%20-%20C%20%5Crangle%7D%0A%5C%5C%7B%5CLarge%20c%20%3D%20%5ClVert%20O%20-%20C%20%5CrVert%5E%7B2%7D%20-%20r%5E%7B2%7D%7D%0A%5C%5C%7B%5CLarge%20%5CDelta%20%3D%20b%5E%7B2%7D%20-%204ac%7D%0A%5Cend%7Balign%2A%7D&mode=inline)
