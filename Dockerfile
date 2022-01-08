FROM node:17.3.0-alpine3.14
WORKDIR /usr/src/app

COPY package*.json ./
RUN npm install

COPY . .
RUN node index.js

CMD [ "node", "upload.js" ]