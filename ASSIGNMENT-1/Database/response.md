# Observation Table:

| #clients 	| Server Response Time(ns)                	|
|----------	|-----------------------------------------	|
|     5    	| 50,000 - 1,50,000 (majorly near 90,000  	|
|     4    	| 40,000 - 1,20,000 (majorly near 80,000) 	|
|     3    	| 30,000 - 90,000 (majorly near 40,000 )  	|
|     2    	| 20,000 - 80,000 (majorly near 40,000 )  	|
|     1    	| 20,000 - 70,000 (majorly near 20,000)   	|

Again 5 clients is too less to overload the server, although the resonse time is overall greater than the echo engine because file opening and reading takes place. The above table states a range of values(in nanosec) and also the most observed response time figure. 
