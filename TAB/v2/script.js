board = document.getElementById('board')
        box = document.getElementsByClassName('box')
        coinb = document.getElementsByClassName('coin_b')
        coinw = document.getElementsByClassName('coin_w')
        // btn_req_fl_s = document.getElementById('btn_req_fl_s')
        fl_scrn = document.getElementById('full_screen')
        lft_pan = document.getElementById('lft_pan')

        function fs_req() {
            fl_scrn.style.display = 'none'
            board.style.display = 'block'
            board.requestFullscreen()
            update_board()
        }


        // win_h = window.innerHeight
        // win_w = window.innerWidth

        // if (win_h > win_w) {
        //     box.style.height = win_w
        //     box.style.width = win_w
        // } else {
        //     box.style.height = win_h
        //     box.style.width = win_h
        // }

        

        POS = [
            'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
            'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
            ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
            ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
            ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
            ' ', ' ', ' ', ' ', ' ', ' ', 'p', ' ',
            'p', 'p', 'p', 'p', 'p', 'p', ' ', 'p',
            'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r']


        function init_coin(coin, at) {
            box[at].innerHTML = ''
            if (coin != ' ') {
                img_elm = document.createElement('img')
                img_elm.width = box[0].offsetHeight / 2 + 9
                img_elm.style.marginTop = box[0].offsetHeight / 2 - box[0].offsetHeight / 4 + 'px'
                img_elm.alt = coin
                mtc = { r: 'rw', R: 'rb', n: 'nw', N: 'nb', b: 'bw', B: 'bb', q: 'qw', Q: 'qb', k: 'kw', K: 'kb', p: 'pw', P: 'pb' }
                img_elm.src = '../img/' + mtc[coin] + '.png'
                box[at].appendChild(img_elm)
            }
        }


        function update_board() {
            for (i = 0; i < 64; i++) {
                init_coin(POS[i], i)
            }
        }

        update_board()