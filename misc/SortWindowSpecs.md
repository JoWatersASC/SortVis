# Sorting Window Class

#### declare struct dim{ x and y } and struct position{ x and y }
## Private Members
- vector of ints
- dimension dim
- position pos

## Public Members
- #### constructor (void)
- #### render function (dim and pos)
    - renders bars representing elems of vector
    - renders execution time of sort, using high res clock
    - renders window
- #### friend colision function (sorting window)
    - takes another window for collisions
    - uses position and dimension members to check
- static pointer list of window objects

### Window Components
- vector bars
- execution time
- dropdown for selection
- logarithmic slider for num elems
- non movable
- non collapsable
- if only one window, non closable

