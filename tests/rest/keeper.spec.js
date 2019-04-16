const chai = require('chai');
const chai_http = require('chai-http');
const should = chai.should();
const events = require('events');
var server = require('./server-interface');

chai.use(chai_http);

const KEEPER_SECRET = "dmVyeS1iYXNpYw=="
const PASSWORDS = {
  good: {
    name: 'see-you-later',
    secret:'aligator'
  },
  no_name: {
    secret: 'no-name',
  },
  no_secret: {
    name: 'no-secret',
  },
  integer_name: {
    name: 42,
    secret: '*'
  },
  integer_secret: {
    name: 'once-more',
    secret: 1
  },
  not_an_object: 'too-bad',
};

describe('Password keeper passwords interface', function () {
  before(function (done) {
    server.start()

    done();
  });

  after(function () {
  });

  describe('POST /keeper', function () {
    it('should\'nt create password without authorization', function(done) {
      chai.request(server)
        .post('/keeper')
        .send(PASSWORDS['good'])
        .end(function (err, res) {
          should.not.exist(err);
          res.should.have.status(401);

          done();
        });
    });

    it('should\'nt create password with wrong authorization', function(done) {
      chai.request(server)
        .post('/keeper')
        .set('Authorization', 'Basic abc' + KEEPER_SECRET)
        .send(PASSWORDS['good'])
        .end(function (err, res) {
          should.not.exist(err);
          res.should.have.status(403);

          done();
        });
    });

    it('should create new password instance', function(done) {
      chai.request(server)
        .post('/keeper')
        .set('Authorization', 'Basic ' + KEEPER_SECRET)
        .send(PASSWORDS['good'])
        .end(function (err, res) {
          should.not.exist(err);
          res.should.have.status(201);

          done();
        });
    });

    it('should\'nt create duplicate password instance', function(done) {
      chai.request(server)
        .post('/keeper')
        .set('Authorization', 'Basic ' + KEEPER_SECRET)
        .send(PASSWORDS['good'])
        .end(function (err, res) {
          should.exist(err);
          err.should.have.status(409);

          done();
        });
    });

    it('should\'nt create password instance without secret', function(done) {
      chai.request(server)
        .post('/keeper')
        .set('Authorization', 'Basic ' + KEEPER_SECRET)
        .send(PASSWORDS['no_secret'])
        .end(function (err, res) {
          should.exist(err);
          err.should.have.status(400);
          done();
        });
    });

    it('should\'nt create password instance without name', function(done) {
      chai.request(server)
        .post('/keeper')
        .set('Authorization', 'Basic ' + KEEPER_SECRET)
        .send(PASSWORDS['no_name'])
        .end(function (err, res) {
          should.exist(err);
          err.should.have.status(400);
          done();
        });
    });

    it('should\'nt create password instance with integer secret', function(done) {
      chai.request(server)
        .post('/keeper')
        .set('Authorization', 'Basic ' + KEEPER_SECRET)
        .send(PASSWORDS['integer_secret'])
        .end(function (err, res) {
          should.exist(err);
          err.should.have.status(400);
          done();
        });
    });

    it('should\'nt create password instance with integer name', function(done) {
      chai.request(server)
        .post('/keeper')
        .set('Authorization', 'Basic ' + KEEPER_SECRET)
        .send(Buffer([PASSWORDS['integer_name']]))
        .end(function (err, res) {
          should.exist(err);
          err.should.have.status(400);
          done();
        });
    });

    it('should\'nt create password instance from string', function(done) {
      chai.request(server)
        .post('/keeper')
        .set('Authorization', 'Basic ' + KEEPER_SECRET)
        .send(PASSWORDS['not_an_object'])
        .end(function (err, res) {
          should.exist(err);
          err.should.have.status(415);
          done();
        });
    });
  });

  describe('GET /keeper', function () {
    it('should read password secret', function(done) {
      chai.request(server)
        .get('/keeper/' + PASSWORDS['good']['name'])
        .set('Authorization', 'Basic ' + KEEPER_SECRET)
        .end(function (err, res) {
          should.not.exist(err);
          res.should.have.status(200);
          res.body.should.be.eql(PASSWORDS['good']['secret']);

          done();
        });
    });

    it('should\'nt read unexisting password instance', function(done) {
      chai.request(server)
        .post('/keeper/not-existing-password')
        .set('Authorization', 'Basic ' + KEEPER_SECRET)
        .end(function (err, res) {
          should.exist(err);
          err.should.have.status(404);

          done();
        });
    });
  });

  describe('PUT /keeper', function () {
    it('should change password instance secret', function(done) {
      chai.request(server)
        .put('/keeper/' + PASSWORDS['good']['name'])
        .set('Authorization', 'Basic ' + KEEPER_SECRET)
        .send(PASSWORDS['good']['name'])
        .end(function (err, res) {
          should.not.exist(err);
          res.should.have.status(204);

          chai.request(server)
            .get('/keeper/' + PASSWORDS['good']['name'])
            .set('Authorization', 'Basic ' + KEEPER_SECRET)
            .end(function (err, res) {
              should.not.exist(err);
              res.should.have.status(200);
              res.body.should.be.eql(PASSWORDS['good']['name']);

              done();
            });
        });
    });

    it('should\'nt change password secret to integer', function(done) {
      chai.request(server)
        .put('/keeper/' + PASSWORDS['good']['name'])
        .set('Authorization', 'Basic ' + KEEPER_SECRET)
        .send(PASSWORDS['integer_name']['name'])
        .end(function (err, res) {
          should.exist(err);
          err.should.have.status(400);

          done();
        });
    });

    it('should\'nt change unexisting password secret', function(done) {
      chai.request(server)
        .put('/keeper/not-existing-password')
        .set('Authorization', 'Basic ' + KEEPER_SECRET)
        .send(PASSWORDS['good']['secret'])
        .end(function (err, res) {
          should.exist(err);
          err.should.have.status(404);

          done();
        });
    });
  });

  describe('DELETE /keeper', function () {
    it('should delete password secret', function(done) {
      chai.request(server)
        .delete('/keeper/' + PASSWORDS['good']['name'])
        .set('Authorization', 'Basic ' + KEEPER_SECRET)
        .end(function (err, res) {
          should.not.exist(err);
          res.should.have.status(204);

          done();
        });
    });

    it('should\'nt delete unexisting password instance', function(done) {
      chai.request(server)
        .delete('/keeper/not-existing-password')
        .set('Authorization', 'Basic ' + KEEPER_SECRET)
        .end(function (err, res) {
          should.exist(err);
          err.should.have.status(404);

          done();
        });
    });
  });
});
