(define counter-simple
	(lambda (n)
		(lambda () (begin (set! n (+ 1 n)) n))))

(define counter
	(lambda (n)
		(begin 
			(define count 
				(lambda () (begin (set! n (+ 1 n)) n)))
			(define reset 
				(lambda () (begin (set! n 0) n)))
			(define dispatch 
				(lambda (d) (if (= d 1) (count) (reset))))
		)
	)
)
