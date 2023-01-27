(define map
	(lambda (f li)
		(if (null? li)
			null
			(cons (f (car li)) (map f (cdr li))))))
