FROM node:17.3.0 as builder
WORKDIR /var/app

COPY generate.js .
COPY data_format.json .
RUN node generate.js

FROM node:17.3.0 as publisher
WORKDIR /var/app
COPY src .
COPY package.json .
COPY --from=builder /var/app/build .

ENV NPM_HOST localhost
ENV NPM_REGISTRY http://localhost
ENV NPM_TOKEN XXX

CMD echo "//$NPM_HOST/:_authToken=\"$NPM_TOKEN\"" > ~/.npmrc; npm publish --registry=$NPM_REGISTRY