logo = document.getElementsByTagName('img')[0]

anime_logo(0,100,20)

function anime_logo(frm,to,delay){
    temp = frm
    interval = setInterval(()=>{
        logo.style.opacity = temp + '%'
        logo.style.transform = "rotate("+ (to-temp+30) +"deg)";
        logo.style.tra

        if(temp == to)
        clearInterval(interval)
        temp++
    },delay)
}