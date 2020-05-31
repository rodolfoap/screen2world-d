%YAML:1.0
---
image_points: !!opencv-matrix
  rows: {{rows}}
  cols: 1
  dt: "2d"
  data: [
    {{#coorduv}}
    {{x}}, {{y}}{{^last}},{{/last}}{{#last}}]{{/last}}
    {{/coorduv}}
world_points: !!opencv-matrix
  rows: {{rows}}
  cols: 1
  dt: "3d"
  data: [
    {{#coordxy}}
    {{x}}, {{y}}, 0.0{{^last}},{{/last}}{{#last}}]{{/last}} # {{c}}
    {{/coordxy}}
