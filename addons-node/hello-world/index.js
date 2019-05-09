// this is how we will require our module
const m = require('./')

const vec1 = new m.Hero(20, "10", 0)
console.log(vec1) // Vector { x: 20, y: 10, z: 0 }

const vec2 = new m.Hero(0, "0", 0)
vec2.x = 30
vec2.z = 100
console.log(vec2) // Vector { x: 30, y: 0, z: 100 }

