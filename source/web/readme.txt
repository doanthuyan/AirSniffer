You can build by yourself or use existing image

1. Build by yourself
Prequesite: you must have docker installed on PC
----------------------
Run the following command to build docker image:

	docker build -t sniffer-status .
----------------------
If you want to run on your local docker engine:
	docker run -dit -p 8080:8080  sniffer-status
----------------------
Run on AAVN openshift:
- Prequesite: you must a have an account on docker hub 
- Create a project within your aavn openshift account
- Tag your image: 
	docker tag sniffer-status <your docker hub account>/sniffer-status:aavn
- Push your image to docker hub:
	docker push <your docker hub account>/sniffer-status:aavn
	
2. Use existing image

Follow steps in here: https://blog.openshift.com/deploying-applications-from-images-in-openshift-part-one-web-console/

In "Image name" field put: thuyan/sniffer-status:aavn or <your docker hub account>/sniffer-status:aavn (if you build by yourself)
