
`AnimationState` define una animacion, es lo que controla la animacion que haya, se crea con createAnimationState() dado un SceneManager
`Animation` es el objeto de la animacion, contiene los tracks de la animacion, en una animacion se puede tener varios tracks.
    - Todos los tracks de la animacion tiene que tener la misma duracion, pero el el numero de keyframes puede ser diferente

`AnimationTrack` es el objeto de track, contiene los keyframes. Se crea dado un objeto de animacion
**El track es lo que hay que asociar a un nodo**

`Keyframe` es el objeto de keyframe que se guarda en el track. Se puede crear un keyframe dado un track con createKeyframe()


Animacion para Sinbad: Dance, lo de resto a mano lol

Lo que tiene Ogre es una cosa que se llama `AnimationStateSet`, en el cual puedes crear los animation state que sea necesario
