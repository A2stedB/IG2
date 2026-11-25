
in vec2 uv0; // Coordenada de textura 0
in float sintime;
out vec2 vUv0; // Coordenadas de textura 0

void main()
{
    vUv0 = uv0;
    vUv0.s = (uv0.s - 0.5) * (sinetime) + 0.5;
    vUv0.t = (uv0.t - 0.5) * (sinetime) + 0.5;
}