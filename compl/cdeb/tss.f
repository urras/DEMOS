        common a,b,ia,ib
        common /xxx/ l1,ri

        integer s(20),c(20)
        complex c1,c2(20)
        double complex dc1, dc2(20)
        logical l1, l2(20)
        double precision d1, d2(20)
        real r1, r2(20)
        integer i1, i2(20)
        character*40 h1,h2(20)

        a = 1.23
        ia = 12345
        l1 = .true.

        do 2 i=1,20
           c(i)=i*2-1
           i1 = i1 + 1
           i2(i) = i1
           r1 = r1 + 0.1
           r2(i) = r1
           d1 = d1 + 0.01
           d2(i) = d1
           l1 = .not.l1
           l2(i) = l1
           c1 = c1 + (0.1,0.2)
           c2(i) = c1
           dc1 = dc1 + (0.01,0.02)
           dc2(i) = dc1
           h1 = '>abcdef<'
           h2(i) = '>>>a<<<'
2       continue
        call aaa(c,s,20)
        stop 1
        end

        subroutine aaa(s,c,n)
        common ca,cb,cia,cib
        common /xxx/ cl1,cri
        integer s(20),c(n),t(20)
        character*50 cc(100)
        complex cr
        data rr/0.0/,cr/(1.0,2.0)/
        cc(1) = 'abcdef'
        cc(2) = 'cc(2)'
        do 1 i=1,20
        t(20-i+1) = i
        s(i) = c(n-i+1)
1       continue
        print *,s,c,cc(1),cc(2)
        return
        end
