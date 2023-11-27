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
        // board.addEventListener('mousedown', (e) => {
        //     trt = e.target
        //     hld_pos = { x: e.clientX, y: e.clientY }
        //     get_over_box(e)
        //     piece_frm = box_num
        //     if (trt.alt) {
        //         console.log(trt.alt, ' from ', piece_frm);
        //         piece_hold = board.addEventListener('mousemove', piece_holf_f)
        //         piece_place = board.addEventListener('mouseup', (y) => {
        //             // console.log('up');
        //             board.removeEventListener('mousemove', piece_holf_f)
        //             if (piece_frm && box_num) {
        //                 temp = POS[piece_frm]
        //                 POS[piece_frm] = ' '
        //                 POS[box_num] = temp
        //             }
        //             // console.log(POS);
        //             box_num = ''
        //             highlight_box()
        //             update_board()
        //         })
        //     }
        // })
        // box_num
        // piece_frm
        // function piece_holf_f(t) {
        //     trt.style.position = 'fixed'
        //     trt.style.top = t.clientY - trt.width + 'px'
        //     trt.style.left = t.clientX - trt.width / 2 + 'px'
        //     get_over_box(t)
        // }
        // function get_over_box(t) {
        //     Xdta = board.offsetWidth - t.clientX;
        //     Ydta = board.offsetHeight - t.clientY;
        //     if (Xdta >= 0 && Ydta >= 0) {
        //         x = 7 - parseInt(Xdta / box[0].offsetWidth)
        //         y = 7 - parseInt(Ydta / box[0].offsetHeight)
        //         x = x == -1 ? 0 : x
        //         y = y == -1 ? 0 : y
        //         box_num = y * 8 + x
        //         highlight_box()
        //     }
        // }
        // function highlight_box() {
        //     for (i = 0; i < 64; i++) {
        //         box[i].style.boxShadow = "";
        //     }
        //     if (box_num != '') {
        //         box[box_num].style.boxShadow = "inset 0 0 15px #f8a100";
        //         // console.log('hold: ', box_num);
        //     }
        // }
